#pragma once

#include "IAction.h"

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class TurnManager;
class Board;

class ActionManager
{
public:
    ActionManager(Board& board, TurnManager& turnManager);
    ActionManager(ActionManager& other) = delete;

    void AddAction(std::unique_ptr<IAction> action, bool immediateApply = true);

    template <typename TAction, typename... Args>
        requires std::derived_from<TAction, IAction>
    void AddAction(Args&&... args)
    {
        std::unique_ptr<IAction> action = std::make_unique<TAction>(std::forward<Args>(args)...);

        AddAction(std::move(action), /*immediateApply = */ true);
    }

    template <typename TAction, typename... Args>
        requires std::derived_from<TAction, IAction>
    void AddActionDeferred(Args&&... args)
    {
        auto action = std::make_unique<TAction>(std::forward<Args>(args)...);

        AddAction(std::move(action), /*immediateApply = */ false);
    }
    void Scrub(int numActions);
    void Reset(bool clearActions = false);

    nlohmann::json ToJson() const;
    void ReadActionsFromJson(const nlohmann::json& j);

private:
    Board& mBoard;
    TurnManager& mTurnManager;

    std::vector<std::unique_ptr<IAction>> mAllActions;
    int mAppliedActionIndex{-1};
};
