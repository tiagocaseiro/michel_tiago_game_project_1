#include "CardZoneRules.h"

#include "Model/CardType.h"

const CardZoneRules& CardZoneRules::TroopDeck()
{
    static CardZoneRules sTroopDeck 
    {
        .mSlotLimit = std::nullopt,
        .mSupportedCardTypes = {CardType::Troop}
    };
    
    return sTroopDeck;
}

const CardZoneRules& CardZoneRules::TacticsDeck()
{
    static CardZoneRules sTacticsDeck 
    {
        .mSlotLimit = std::nullopt,
        .mSupportedCardTypes = {CardType::TacticTroop, CardType::TacticGuile, CardType::TacticEnvironment}
    };
    
    return sTacticsDeck;
}

const CardZoneRules& CardZoneRules::PlayerHand() 
{
    static CardZoneRules sPlayerHand 
    {
        .mSlotLimit = 7,
        .mSupportedCardTypes = {}
    };
    
    return sPlayerHand;
}

const CardZoneRules& CardZoneRules::FlagTroops() 
{
    static CardZoneRules sFlagTroops 
    {
        .mSlotLimit = 3,
        .mSupportedCardTypes = {CardType::TacticTroop, CardType::Troop}
    };
    
    return sFlagTroops;
}

const CardZoneRules& CardZoneRules::FlagTactics() 
{
    static CardZoneRules sFlagTactics 
    {
        .mSlotLimit = std::nullopt,
        .mSupportedCardTypes = {CardType::TacticEnvironment}
    };
    
    return sFlagTactics;
}

const CardZoneRules& CardZoneRules::DiscardPile() 
{
    static CardZoneRules sDiscardPile 
    {
        .mSlotLimit = std::nullopt,
        .mSupportedCardTypes = {}
    };
    
    return sDiscardPile;
}
