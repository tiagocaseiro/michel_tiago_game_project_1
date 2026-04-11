#pragma once

#include "TurnManager.h"

#include "Actions/IAction.h"

class IAction;

TurnManager::TurnManager(const Player& player1, const Player& player2)
    : mPlayers({player1, player2
    })
{
}

void TurnManager::OnActionPlayed(IAction& playedAction)
{
    mTurnPhase = playedAction.FollowingTurnPhase();
    if (mTurnPhase == TurnPhase::PlayCard)
    {
        AdvanceTurn();
    }
}

void TurnManager::UndoAction(IAction& undoneAction, IAction* actionBefore)
{
    if (!undoneAction.AdvancesTurn())
    {
        return;
    }

    if (actionBefore == nullptr)
    {
        mTurnPhase = TurnPhase::PlayCard;
        mTurnNumber = 1;
        mPlayerIndex = 0;
        return;
    }

    mTurnPhase = actionBefore->FollowingTurnPhase();
    // We just moved back to the last phase of last turn
    if(mTurnPhase == TurnPhase::ReplenishHand)
    {
        RegressTurn();
    }
}

void TurnManager::AdvanceTurn()
{
    if(++mPlayerIndex >= mPlayers.size())
    {
        mPlayerIndex = 0;
        ++mTurnNumber;
    }
}

void TurnManager::RegressTurn()
{
    if(--mPlayerIndex < 0)
    {
        mPlayerIndex = mPlayers.size() - 1;
        --mTurnNumber;
    }
}