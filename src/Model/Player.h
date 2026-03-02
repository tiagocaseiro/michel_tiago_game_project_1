#pragma once

#include "CardZone.h"
#include "Tools/TGUID.h"

#include <nlohmann/json_fwd.hpp>

struct Player
{
    CardZone mPlayerHand{CardZoneRules::PlayerHand()};
    TGUID<Player> mGuid{TGUID<Player>::Generate()};

    nlohmann::json ToJson() const;
};