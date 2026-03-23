#include "ActionRulesManager.h"

#include "Model/TurnManager.h"
#include "Actions/IAction.h"

std::vector<IAction> ValidActions::GenerateValidActions(const Board& board, const TurnManager& currentTurn)
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

std::vector<IAction> ValidActions::GenerateDrawPhaseActions(const Board& board, const TurnManager& currentTurn)
{

}

std::vector<IAction> ValidActions::GeneratePlayPhaseActions(const Board& board, const TurnManager& currentTurn)
{
}