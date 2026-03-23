#pragma once
#include "Player.h"
#include "Tools/Logging.h"

#include <vector>

enum TurnPhase
{
    PlayCard,
    ResolveTactic,
    ReplenishHand
};

class TurnManager
{
public:
    TurnManager(const Player& player1, const Player& player2)
        : mPlayers({player1, player2
        })
    {
    }

    const Player& GetActivePlayer() const { return mPlayers[mPlayerIndex]; }
    int GetTurnNumber() const { return mTurnNumber; }
    int GetPlayerIndex() const { return mPlayerIndex; }
    TurnPhase GetTurnPhase() const { return mTurnPhase; }

    void OnCardPlayer(bool isTactic)
    {
        if (isTactic)
        {
            mTurnPhase = ResolveTactic;
        }
        else
        {
            mTurnPhase = ReplenishHand;
        }
    }

    void OnTacticResolved()
    {
        mTurnPhase = ReplenishHand;
    }

    void AdvanceTurn()
    {
        // #TODO: Add an assert / logIf function
        if (mTurnPhase != ReplenishHand)
        {
            Logging::LogError("Should not be able to skip replenish hand phase");
        }
        mTurnPhase = PlayCard;
        if(++mPlayerIndex >= mPlayers.size())
        {
            mPlayerIndex = 0;
            ++mTurnNumber;
        }
    }

private:
    std::array<std::reference_wrapper<const Player>, 2> mPlayers;
    int mTurnNumber{1};
    int mPlayerIndex{0};
    TurnPhase mTurnPhase{PlayCard};
};