#pragma once

#include <vector>
#include <memory>

class IAction;
struct Board;
class TurnManager;

class ValidActions
{
    static std::vector<std::unique_ptr<IAction>> GenerateValidActions(Board& board, TurnManager& currentTurn);

    static std::vector<std::unique_ptr<IAction>> GenerateDrawPhaseActions(Board& board, TurnManager& currentTurn);
    static std::vector<std::unique_ptr<IAction>> GeneratePlayPhaseActions(Board& board, TurnManager& currentTurn);
};
