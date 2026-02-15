#pragma once

enum class CardType
{
    Troop,
    TacticTroop,
    TacticEnvironment,
    TacticGuile
};

bool IsTacticsCard(CardType type)
{
    switch (type)
    {
        case CardType::Troop:
            return false;
        case CardType::TacticTroop:
        case CardType::TacticEnvironment:
        case CardType::TacticGuile:
            return true;
    }

    // #TODO: Add logging library 
    // LOG(Unsupported cardtype)
    return false;
}