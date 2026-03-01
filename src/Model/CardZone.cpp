#include "CardZone.h"

#include "CardType.h"
#include "Rules/CardZoneRules.h"

bool CardZone::CanHoldCard(const Card& card) const
{
    // #TODO_CORE: We need a separation between Card and TroopCard
    // A Card needs a CardType member that we need to read here
    if (!mRules.AcceptsCard(CardType::Troop))
    {
        return false;
    }

    if (!mRules.mSlotLimit)
    {
        return true;
    }

    return mCards.size() < *mRules.mSlotLimit;
}