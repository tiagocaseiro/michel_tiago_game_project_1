#pragma once

#include <vector>

#include "Flag.h"

struct Player;

struct Board
{
    Player* mPlayer1 = nullptr;
    Player* mPlayer2 = nullptr;
    std::vector<Flag> mFlags;
    CardZone mTroopDeck;
    CardZone mTacticsDeck;
    CardZone mDiscardPile;
};