#pragma once

#include "CardType.h"

#include <optional>
#include <vector>
#include <algorithm>

enum class CardType;

struct CardZone
{
    std::optional<int> mSlotLimit;
    std::vector<CardType> mSupportedCardTypes;

    [[nodiscard]] bool AcceptsCard(CardType type)
    {
        if (mSupportedCardTypes.empty())
        {
            return true;
        }

        return std::ranges::contains(mSupportedCardTypes, type);
    }
};