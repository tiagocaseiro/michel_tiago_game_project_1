#include "Data.h"

namespace Common
{
    static SDL_Renderer* sRenderer       = nullptr;
    static TTF_TextEngine* sTextRenderer = nullptr;

    void SetRenderer(SDL_Renderer* renderer) { sRenderer = renderer; }
    SDL_Renderer* GetRenderer() { return sRenderer; }

    void SetTextRenderer(TTF_TextEngine* textRenderer) { sTextRenderer = textRenderer; }
    TTF_TextEngine* GetTextRenderer() { return sTextRenderer; }
} // namespace Common