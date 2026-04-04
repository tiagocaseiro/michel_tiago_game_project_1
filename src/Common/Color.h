#pragma once

#include <SDL3/SDL.h>

namespace Common
{
    namespace Color
    {
        constexpr auto Transparent = SDL_FColor(0.0, 0.0, 0.0, 0.0f);
        constexpr auto White       = SDL_FColor(1.0, 1.0, 1.0, 1.0f);
        constexpr auto Black       = SDL_FColor(0.0, 0.0, 0.0, 1.0f);
        constexpr auto Red         = SDL_FColor(1.0, 0.0, 0.0, 1.0f);
        constexpr auto Green       = SDL_FColor(0.0, 1.0, 0.0, 1.0f);
        constexpr auto Blue        = SDL_FColor(0.0, 0.0, 1.0, 1.0f);
        constexpr auto Yellow      = SDL_FColor(0.0, 1.0, 1.0, 1.0f);
        constexpr auto Purple      = SDL_FColor(0.5, 0.0, 0.5, 1.0f);
        constexpr auto Brown       = SDL_FColor(0.6, 0.3, 0.0, 1.0f);
    } // namespace Color

    inline SDL_FColor StringToColor(const std::string& colorId)
    {
        if("Transparent" == colorId)
        {
            return Color::Transparent;
        }
        if("White" == colorId)
        {
            return Color::White;
        }
        if("Black" == colorId)
        {
            return Color::Black;
        }
        if("Red" == colorId)
        {
            return Color::Red;
        }
        if("Green" == colorId)
        {
            return Color::Green;
        }
        if("Blue" == colorId)
        {
            return Color::Blue;
        }
        if("Yellow" == colorId)
        {
            return Color::Yellow;
        }
        if("Purple" == colorId)
        {
            return Color::Purple;
        }
        if("Brown" == colorId)
        {
            return Color::Brown;
        }
        return Color::Black;
    }
} // namespace Common
