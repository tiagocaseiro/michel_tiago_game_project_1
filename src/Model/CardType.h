#pragma once

#include "Tools/Logging.h"

enum class CardType
{
    Troop,
    TacticTroop,
    TacticEnvironment,
    TacticGuile
};

bool IsTacticsCard(CardType type);