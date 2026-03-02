#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>

struct TTF_Font;

using TextureUniquePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;

namespace Common
{
    TextureUniquePtr LoadTexture(const std::string& imagePath);
    TextureUniquePtr LoadTextTexture(TTF_Font& font, SDL_FColor color, const std::string& text);
    TextureUniquePtr EmptyTexture();
} // namespace Common
