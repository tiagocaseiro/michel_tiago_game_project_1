#include "CardType.h"

bool IsTacticsCard(CardType type) 
{ 
    switch(type)
    {
        case CardType::Troop:
            return false;
        case CardType::TacticTroop:
        case CardType::TacticEnvironment:
        case CardType::TacticGuile:
            return true;
    }

    Logging::LogWarning("Unsupported CardType");
    return false; 
}