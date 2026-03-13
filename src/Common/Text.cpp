#include "Text.h"

#include <memory>
#include <unordered_map>

#include <SDL3/SDL.h>

#include "Common/Data.h"
#include "Tools/Logging.h"

using FontUniquePtr = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>;

std::unordered_map<std::string, std::unordered_map<int, FontUniquePtr>> sFonts;

namespace Common
{
    TTF_Font* GetFont(const std::string& fontPath, const int size)
    {
        std::unordered_map<int, FontUniquePtr>& fontMap = sFonts[fontPath];

        if(fontMap.contains(size))
        {
            return fontMap.at(size).get();
        }

        FontUniquePtr font = FontUniquePtr(TTF_OpenFont(fontPath.data(), size), &TTF_CloseFont);

        if(font == nullptr)
        {
            Logging::LogInfo("Failed to load font path {}", fontPath);
            return nullptr;
        }

        auto it = fontMap.emplace(size, std::move(font));

        return it.first->second.get();
    }

    void UnloadAllFonts() { sFonts.clear(); }

    SDLTextUniquePtr CreateText(const std::string& fontPath, const int size, const std::string& text)
    {
        TTF_Text* sdlText = TTF_CreateText(GetTextRenderer(), GetFont(fontPath, size), text.c_str(), text.size());

        return SDLTextUniquePtr(sdlText, &TTF_DestroyText);
    }

    SDLTextUniquePtr EmptyText() { return SDLTextUniquePtr(nullptr, &TTF_DestroyText); }

} // namespace Common