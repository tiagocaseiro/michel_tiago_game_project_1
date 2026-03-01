#pragma once

#include "CardZone.h"
#include "Tools/TGUID.h"

struct Player
{
    CardZone mPlayerHand{CardZoneRules::PlayerHand()};
    TGUID<Player> mGuid{TGUID<Player>::Generate()};
};