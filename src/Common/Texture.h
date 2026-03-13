#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>

struct TTF_Font;

using SDLTextureUniquePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;

namespace Common
{
    SDLTextureUniquePtr LoadTexture(const std::string& imagePath);
    SDLTextureUniquePtr LoadTextTexture(TTF_Font& font, SDL_FColor color, const std::string& text);
    SDLTextureUniquePtr EmptyTexture();
} // namespace Common
