#include "Texture.h"

#include <unordered_map>

#include <SDL3_ttf/SDL_ttf.h>

#include "Common/Data.h"
#include "Tools/Logging.h"

namespace Common
{
    using SurfaceUniquePtr = std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)>;

    TextureUniquePtr LoadTexture(const std::string& imagePath)
    {
        SDL_Renderer* renderer = Common::GetRenderer();

        if(renderer == nullptr)
        {
            Logging::LogError("Renderer is not initialized");
            return EmptyTexture();
        }

        SurfaceUniquePtr surface = SurfaceUniquePtr(SDL_LoadSurface(imagePath.data()), SDL_DestroySurface);

        if(surface == nullptr)
        {
            Logging::LogError("Failed to load texture surface data for path {}: {}", imagePath, SDL_GetError());
            return EmptyTexture();
        }

        TextureUniquePtr texture =
            TextureUniquePtr(SDL_CreateTextureFromSurface(renderer, surface.get()), &SDL_DestroyTexture);

        if(texture == nullptr)
        {
            Logging::LogError("Failed to load texture data for path {}: {}", imagePath, SDL_GetError());
        }

        return texture;
    }

    TextureUniquePtr LoadTextTexture(TTF_Font& font, SDL_FColor color, const std::string& text)
    {
        SDL_Renderer* renderer = Common::GetRenderer();

        if(renderer == nullptr)
        {
            Logging::LogError("Renderer is not initialized");
            return EmptyTexture();
        }

        const SDL_Color newColor{
            .r = static_cast<Uint8>(color.r * 255.f),
            .g = static_cast<Uint8>(color.g * 255.f),
            .b = static_cast<Uint8>(color.b * 255.f),
            .a = static_cast<Uint8>(color.a * 255.f),
        };

        SurfaceUniquePtr surface =
            SurfaceUniquePtr(TTF_RenderText_Blended(&font, text.c_str(), 0, newColor), SDL_DestroySurface);

        if(surface == nullptr)
        {
            Logging::LogError("Failed to load texture surface data for font");
            return EmptyTexture();
        }

        TextureUniquePtr texture =
            TextureUniquePtr(SDL_CreateTextureFromSurface(renderer, surface.get()), &SDL_DestroyTexture);

        if(texture == nullptr)
        {
            Logging::LogError("Failed to load texture data for font");
        }

        return texture;
    }

    TextureUniquePtr EmptyTexture() { return TextureUniquePtr(nullptr, &SDL_DestroyTexture); }
} // namespace Common
