#pragma once

#include <vector>

#include "Card.h"

struct InitialSetup;

class CardManager
{
public:
    CardManager(const InitialSetup& initialSetup);

private:
    std::vector<Card> mAllCards;
};