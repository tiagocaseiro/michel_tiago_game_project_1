#include "Data.h"

namespace Common
{
    static SDL_Renderer* sRenderer = nullptr;

    void SetRenderer(SDL_Renderer* renderer) { sRenderer = renderer; }
    SDL_Renderer* GetRenderer() { return sRenderer; }
} // namespace Common