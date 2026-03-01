#pragma once

#include <SDL3/SDL.h>

namespace Common
{
    namespace Color
    {
        constexpr auto TRANSPARENT = SDL_FColor(0.0, 0.0, 0.0, 0.0f);
        constexpr auto WHITE       = SDL_FColor(1.0, 1.0, 1.0, 1.0f);
        constexpr auto BLACK       = SDL_FColor(0.0, 0.0, 0.0, 1.0f);
        constexpr auto RED         = SDL_FColor(1.0, 0.0, 0.0, 1.0f);
        constexpr auto GREEN       = SDL_FColor(0.0, 1.0, 0.0, 1.0f);
        constexpr auto BLUE        = SDL_FColor(0.0, 0.0, 1.0, 1.0f);
    } // namespace Color

} // namespace Common
