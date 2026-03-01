#include "Fonts.h"

#include <memory>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Tools/Logging.h"

using FontUniquePtr = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>;

std::unordered_map<std::string, FontUniquePtr> sFonts;

namespace Common
{
    void LoadFont(const std::string& fontPath, const float size)
    {
        auto found = sFonts.find(fontPath);
        if(found != std::end(sFonts))
        {
            return;
        }

        FontUniquePtr font = FontUniquePtr(TTF_OpenFont(fontPath.data(), size), &TTF_CloseFont);

        if(font == nullptr)
        {
            Logging::LogInfo("Failed to load font path {}", fontPath);
            return;
        }

        sFonts.emplace(fontPath, std::move(font));
    }

    void UnloadAllFonts() { sFonts.clear(); }

    TTF_Font* GetFont(const std::string& fontPath)
    {
        auto found = sFonts.find(fontPath);
        if(found == std::end(sFonts))
        {
            return nullptr;
        }

        return found->second.get();
    }
} // namespace Common