#pragma once

struct SDL_Renderer;
struct TTF_TextEngine;

namespace Common
{
    void SetRenderer(SDL_Renderer* renderer);
    SDL_Renderer* GetRenderer();

    void SetTextRenderer(TTF_TextEngine* textRenderer);
    TTF_TextEngine* GetTextRenderer();

} // namespace Common