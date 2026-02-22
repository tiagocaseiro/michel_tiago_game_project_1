#pragma once

#include "CardZone.h"
#include "Rules/CardZoneRules.h"

struct Player;

struct Flag
{
    CardZone mPlayer1Tactics{CardZoneRules::FlagTactics()};
    CardZone mPlayer1Troops{CardZoneRules::FlagTroops()};
    CardZone mPlayer2Tactics{CardZoneRules::FlagTactics()};
    CardZone mPlayer2Troops{CardZoneRules::FlagTroops()};
    Player* mWinner = nullptr;
};