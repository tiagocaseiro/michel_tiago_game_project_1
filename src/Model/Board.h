#pragma once

#include <vector>

#include "Flag.h"

struct Player;

struct Board
{
    Player* mPlayer1 = nullptr;
    Player* mPlayer2 = nullptr;
    std::vector<Flag> mFlags;
    // #TODO: Separate CardZone from CardZoneRules
    // CardZoneRules lives in Rules folder alongside the others
    // CardZone holds a pointer to a CardZoneRules + knows what cards it currently contains, optionally has an id/name
    CardZone mTroopDeck;
    CardZone mTacticsDeck;
    CardZone mDiscardPile;
};