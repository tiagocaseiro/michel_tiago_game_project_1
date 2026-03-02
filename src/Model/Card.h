#pragma once
#include "Tools/TGUID.h"

#include <nlohmann/json_fwd.hpp>

enum class CardColour;

struct Card
{
    Card(int power, CardColour colour) : mPower(power), mColour(colour) {}

    nlohmann::json ToJson() const;

    int mPower;
    CardColour mColour;
    TGUID<Card> mGuid{TGUID<Card>::Generate()};
};