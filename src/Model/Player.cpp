#include "Player.h"

#include <nlohmann/json.hpp>

nlohmann::json Player::ToJson() const
{
    nlohmann::json j;

    j["ID"] = mGuid.ID();
    nlohmann::json hand;
    for (const auto& card : mPlayerHand.mCards)
    {
        hand.push_back(card.ToJson());
    }
    j["Hand"] = hand;
    return j;
}