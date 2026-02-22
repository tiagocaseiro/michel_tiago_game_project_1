#pragma once

#include <algorithm>
#include <optional>
#include <vector>

enum class CardType;

struct CardZone
{
    std::optional<int> mSlotLimit;
    std::vector<CardType> mSupportedCardTypes;

    [[nodiscard]] bool AcceptsCard(CardType type)
    {
        if(mSupportedCardTypes.empty())
        {
            return true;
        }

        return std::ranges::contains(mSupportedCardTypes, type);
    }
};