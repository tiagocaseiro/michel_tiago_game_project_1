#pragma once

enum class CardColour
{
    Red,
    Green,
    Blue,
    Yellow,
    Purple,
    Brown
};

namespace EnumUtil
{
    constexpr std::string ToString(CardColour colour)
    {
        switch(colour)
        {
            case CardColour::Red:
                return "Red";
            case CardColour::Green:
                return "Green";
            case CardColour::Blue:
                return "Blue";
            case CardColour::Yellow:
                return "Yellow";
            case CardColour::Purple:
                return "Purple";
            case CardColour::Brown:
                return "Brown";
            default:
                return "Unknown";
        }
    }
}