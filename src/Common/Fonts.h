#pragma once

#include <string>

struct TTF_Font;

namespace Common
{
    void LoadFont(const std::string& fontPath, const float size);
    void UnloadAllFonts();
    TTF_Font* TryGetFont(const std::string& fontPath);
} // namespace Common
