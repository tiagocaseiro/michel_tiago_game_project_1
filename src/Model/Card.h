#pragma once

enum class CardColour;

struct Card
{
    Card(int power, CardColour colour) : mPower(power), mColour(colour) {}

    int mPower;
    CardColour mColour;
};