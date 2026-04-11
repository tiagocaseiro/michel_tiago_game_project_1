#include "ActionManager.h"

#include "ActionFactory.h"
#include "Model/Board.h"
#include "Model/TurnManager.h"

#include "Tools/Logging.h"

ActionManager::ActionManager(Board& board, TurnManager& turnManager)
    : mBoard(board),
      mTurnManager(turnManager)
{
}

void ActionManager::AddAction(std::unique_ptr<IAction> action, const bool immediateApply /*= true*/)
{
    if(action == nullptr)
    {
        Logging::LogWarning("Trying to apply invalid action");
        return;
    }

    mAllActions.push_back(std::move(action));
    if(immediateApply)
    {
        if(mAppliedActionIndex != mAllActions.size() - 2)
        {
            Logging::LogDebug("Trying to apply action, but model is not up-to-date");
            return;
        }
        auto& newAction = *mAllActions.back();
        newAction.Apply();
        mTurnManager.OnActionPlayed(newAction);
        ++mAppliedActionIndex;
    }
}

void ActionManager::Scrub(const int numActions)
{
    const auto totalActions = static_cast<int>(mAllActions.size());
    int targetAppliedIndex  = mAppliedActionIndex + numActions;
    if(targetAppliedIndex < 0 || targetAppliedIndex >= totalActions)
    {
        Logging::LogDebug(
            "Model is on index {} out of {} actions total. Trying to scrub {} actions, which would put us at {}, "
            "which is not possible",
            mAppliedActionIndex, totalActions, numActions, targetAppliedIndex);
    }
    targetAppliedIndex = std::clamp(targetAppliedIndex, 0, totalActions);

    if(numActions > 0)
    {
        while(mAppliedActionIndex < targetAppliedIndex)
        {
            ++mAppliedActionIndex;
            auto& newAction = *mAllActions[mAppliedActionIndex];
            newAction.Apply();
            mTurnManager.OnActionPlayed(newAction);

        }
    }
    else
    {
        while(mAppliedActionIndex > targetAppliedIndex)
        {
            auto& lastAction   = *mAllActions[mAppliedActionIndex];
            auto* actionBefore = mAppliedActionIndex > 0 ? mAllActions[mAppliedActionIndex - 1].get() : nullptr;
            lastAction.Undo();
            mTurnManager.UndoAction(lastAction, actionBefore);
            --mAppliedActionIndex;
        }
    }
}

void ActionManager::Reset(bool clearActions /*= false*/)
{
    if(mAppliedActionIndex == -1)
    {
        return;
    }

    Scrub(-mAppliedActionIndex);
    if(clearActions)
    {
        mAllActions.clear();
    }
}

nlohmann::json ActionManager::ToJson() const
{
    nlohmann::json j;
    j["AppliedActionIndex"] = mAppliedActionIndex;

    // Serialize all actions
    nlohmann::json actionsArray = nlohmann::json::array();
    for(const auto& action : mAllActions)
    {
        nlohmann::json actionJson;
        actionJson["Type"] = action->GetTypeName();
        actionJson["Data"] = action->ToJson();
        actionsArray.push_back(actionJson);
    }
    j["Actions"] = actionsArray;

    return j;
}

void ActionManager::ReadActionsFromJson(const nlohmann::json& j)
{
    if(!mAllActions.empty())
    {
        Logging::LogError("Cannot load new board without clearing previous one first! Call reset with clear = true!");
        return;
    }
    // Load all actions first
    const auto& actionsArray = j.at("Actions");
    for(const auto& actionJson : actionsArray)
    {
        std::string actionType = actionJson.at("Type").get<std::string>();
        const auto& actionData = actionJson.at("Data");

        // Use the ActionFactory to create the action
        auto action = ActionFactoryRegistry::Instance().Create(actionType, mBoard, actionData);
        if(action)
        {
            mAllActions.push_back(std::move(action));
        }
        else
        {
            Logging::LogWarning("Failed to create action of type: {}", actionType);
        }
    }
    const int appliedActionIndex = j["AppliedActionIndex"].get<int>();

    // Replay all actions to restore the correct state
    Scrub(appliedActionIndex);
}