#include "Model/Board.h"
#include "Rules/InitialSetup.h"
#include "Board.h"

Board::Board(Player& p1, Player& p2, const InitialSetup& setup)
: mPlayer1(p1)
, mPlayer2(p2)
, mFlags(std::vector<Flag>(setup.mNumFlags, Flag{}))
{
    ConstructTroopsDeck(setup);
    ConstructTacticsDeck(setup);
}

void Board::ConstructTroopsDeck(const InitialSetup& setup) 
{
    for (const auto& cpc : setup.mCardsPerColour)
    {
        for (int cardPower = 1; cardPower <= cpc.second; ++cardPower)
        {
            mTroopDeck.mCards.emplace_back(cardPower, cpc.first);
        }
    }
}

void Board::ConstructTacticsDeck(const InitialSetup& /*setup*/) 
{
    // #TODO_TACTICS
    return;
}
