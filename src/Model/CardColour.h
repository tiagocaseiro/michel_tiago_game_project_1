#pragma once

#include "Common/Color.h"

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

    constexpr auto ToColor(CardColour colour)
    {
        switch(colour)
        {
            case CardColour::Red:
                return Common::Color::Red;
            case CardColour::Green:
                return Common::Color::Green;
            case CardColour::Blue:
                return Common::Color::Blue;
            case CardColour::Yellow:
                return Common::Color::Yellow;
            case CardColour::Purple:
                return Common::Color::Purple;
            case CardColour::Brown:
                return Common::Color::Brown;
            default:
                return Common::Color::Black;
        }
    }
} // namespace EnumUtil