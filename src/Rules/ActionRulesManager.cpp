#include "ActionRulesManager.h"

#include "Actions/DrawCardsAction.h"
#include "Model/TurnManager.h"
#include "Actions/IAction.h"

std::vector<std::unique_ptr<IAction>> ValidActions::GenerateValidActions(Board& board, TurnManager& currentTurn)
{
    switch (currentTurn.GetTurnPhase())
    {
        case ReplenishHand:
            return GenerateDrawPhaseActions(board, currentTurn);
        case PlayCard:
            return GeneratePlayPhaseActions(board, currentTurn);
        case ResolveTactic:
            Logging::LogWarning("Tactics not yet implemented, we should not be in a tactics phase. #TODO");
            return {};
    }

    return {};
}

std::vector<std::unique_ptr<IAction>> ValidActions::GenerateDrawPhaseActions(Board& board, TurnManager& currentTurn)
{
    std::vector<std::unique_ptr<IAction>> actions;
    auto& currentPlayer = currentTurn.GetActivePlayer();
    auto& tacticsDeck = board.mTacticsDeck;
    auto& troopDeck = board.mTroopDeck;

    auto tacticsDraw = std::make_unique<DrawCardsActionParams>(tacticsDeck, currentPlayer, 1);
    auto troopsDraw = std::make_unique<DrawCardsActionParams>(troopDeck, currentPlayer, 1);
    auto tacticsAction = std::make_unique<DrawCardsAction>(board, std::move(tacticsDraw));
    auto troopsAction = std::make_unique<DrawCardsAction>(board, std::move(troopsDraw));

    actions.push_back(std::move(tacticsAction));
    actions.push_back(std::move(troopsAction));
    return actions;
}

std::vector<std::unique_ptr<IAction>> ValidActions::GeneratePlayPhaseActions(Board& board, TurnManager& currentTurn)
{
    return {};
}