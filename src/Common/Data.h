#pragma once

struct SDL_Renderer;

namespace Common
{
    void SetRenderer(SDL_Renderer* renderer);
    SDL_Renderer* GetRenderer();
} // namespace Common