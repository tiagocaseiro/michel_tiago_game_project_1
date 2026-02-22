#pragma once

#include "Model/Card.h"

#include <algorithm>
#include <optional>
#include <vector>

enum class CardType;
struct CardZoneRules;

struct CardZone
{
    explicit CardZone(const CardZoneRules& rules) : mRules(rules) {}

    const CardZoneRules& mRules;
    // #TODO: Reference wrapper or pointer?
    std::vector<Card> mCards;
};