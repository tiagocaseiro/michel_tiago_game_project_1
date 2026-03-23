#pragma once

#include <vector>

class IAction;
struct Board;
class TurnManager;

class ValidActions
{
    static std::vector<IAction> GenerateValidActions(const Board& board, const TurnManager& currentTurn);

    static std::vector<IAction> GenerateDrawPhaseActions(const Board& board, const TurnManager& currentTurn);
    static std::vector<IAction> GeneratePlayPhaseActions(const Board& board, const TurnManager& currentTurn);
};
