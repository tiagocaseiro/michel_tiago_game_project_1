#pragma once

#include <vector>

#include "Flag.h"

struct Player;

struct Board
{
    Player* mPlayer1;
    Player* mPlayer2;
    std::vector<Flag> mFlags;
    CardZone mTroopDeck;
    CardZone mTacticsDeck;
    CardZone mDiscardPile;
};