#pragma once

#include "Player.h"
#include "Actions/IAction.h"
#include "Tools/Logging.h"

#include <vector>
#include <functional>
#include <array>

enum class TurnPhase
{
    PlayCard,
    ResolveTactic,
    ReplenishHand
};

class TurnManager
{
public:
    TurnManager(const Player& player1, const Player& player2);

    Player& GetActivePlayer() { return const_cast<Player&>(mPlayers[mPlayerIndex].get()); } // #TODO: REMOVE CONST CAST once going through player manager
    const Player& GetActivePlayer() const { return mPlayers[mPlayerIndex]; }
    int GetTurnNumber() const { return mTurnNumber; }
    int GetPlayerIndex() const { return mPlayerIndex; }
    TurnPhase GetTurnPhase() const { return mTurnPhase; }

    void OnActionPlayed(IAction&);
    void AdvanceTurn();
    void UndoAction(IAction& undoneAction, IAction* actionBefore);
    void RegressTurn();

private:
    std::array<std::reference_wrapper<const Player>, 2> mPlayers;
    int mTurnNumber{1};
    int mPlayerIndex{0};
    TurnPhase mTurnPhase{TurnPhase::PlayCard};
};