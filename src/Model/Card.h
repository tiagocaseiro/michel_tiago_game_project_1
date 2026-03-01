#pragma once
#include "Tools/TGUID.h"

enum class CardColour;

struct Card
{
    Card(int power, CardColour colour) : mPower(power), mColour(colour) {}

    int mPower;
    CardColour mColour;
    TGUID<Card> mGuid{TGUID<Card>::Generate()};
};