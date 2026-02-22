#pragma once

#include <algorithm>
#include <optional>
#include <vector>

enum class CardType;

struct CardZoneRules
{
    std::optional<int> mSlotLimit;
    std::vector<CardType> mSupportedCardTypes;

    static const CardZoneRules& TroopDeck();
    static const CardZoneRules& TacticsDeck();
    static const CardZoneRules& PlayerHand();
    static const CardZoneRules& FlagTroops();
    static const CardZoneRules& FlagTactics();
    static const CardZoneRules& DiscardPile();

    [[nodiscard]] bool AcceptsCard(CardType type)
    {
        if(mSupportedCardTypes.empty())
        {
            return true;
        }

        return std::ranges::contains(mSupportedCardTypes, type);
    }
};