#include "Card.h"

#include "CardColour.h"

#include <nlohmann/json.hpp>

nlohmann::json Card::ToJson() const
{
    nlohmann::json j;
    j["Colour"] = NCardColour::ToString(mColour);
    j["Power"] = mPower;

    return j;
}