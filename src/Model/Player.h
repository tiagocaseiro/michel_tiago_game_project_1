#pragma once

#include "CardZone.h"

struct Player
{
    CardZone mPlayerHand{CardZoneRules::PlayerHand()};
};