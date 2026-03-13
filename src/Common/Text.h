#pragma once

#include <memory>
#include <string>

#include <SDL3_ttf/SDL_ttf.h>

struct TTF_Font;
struct TTF_Text;

using SDLTextUniquePtr = std::unique_ptr<TTF_Text, decltype(&TTF_DestroyText)>;

namespace Common
{
    void UnloadAllFonts();

    SDLTextUniquePtr CreateText(const std::string& fontPath, const int size, const std::string& text);
    SDLTextUniquePtr EmptyText();

} // namespace Common
