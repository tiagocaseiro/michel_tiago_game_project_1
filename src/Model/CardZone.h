#pragma once

#include "Model/Card.h"
#include "Tools/TGUID.h"

#include <algorithm>
#include <optional>
#include <vector>

enum class CardType;
struct CardZoneRules;

struct CardZone
{
    explicit CardZone(const CardZoneRules& rules) : mRules(rules) {}

    bool CanHoldCard(const Card& card) const;

    const CardZoneRules& mRules;
    // #TODO: Reference wrapper or pointer?
    std::vector<Card> mCards;
    TGUID<CardZone> mGuid{TGUID<CardZone>::Generate()};
};