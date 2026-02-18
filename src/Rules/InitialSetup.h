#pragma once

#include <unordered_map>

#include "Model/CardColour.h"

struct InitialSetup
{
    std::unordered_map<CardColour, int> mCardsPerColour{
        {CardColour::Blue, DEFAULT_CARDS_PER_COLOUR},
        {CardColour::Brown, DEFAULT_CARDS_PER_COLOUR},
        {CardColour::Red, DEFAULT_CARDS_PER_COLOUR},
        {CardColour::Yellow, DEFAULT_CARDS_PER_COLOUR},
        {CardColour::Purple, DEFAULT_CARDS_PER_COLOUR},
        {CardColour::Green, DEFAULT_CARDS_PER_COLOUR}
    };
    int mInitialCardsPerPlayer{7};
    int mNumFlags{9};

private:
    static const int DEFAULT_CARDS_PER_COLOUR{10};
};