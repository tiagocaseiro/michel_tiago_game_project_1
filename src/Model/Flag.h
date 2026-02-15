#pragma once

#include "CardZone.h"

struct Player;

struct Flag
{
    CardZone mPlayer1Tactics;
    CardZone mPlayer1Troops;
    CardZone mPlayer2Tactics;
    CardZone mPlayer2Troops;
    Player* mWinner;
};