#include "UIMaterial.h"

#include "imgui.h"

#include "Common/Data.h"
#include "Common/Text.h"

namespace UI
{
    void Material::SetImagePath(const std::string& texturePath)
    {
        if(mTexturePath != texturePath)
        {
            mTexturePath   = texturePath;
            mTextureHandle = Common::LoadTexture(mTexturePath);
        }
    }

    void Material::Draw() const
    {
        if(mVisible == true)
        {
            SDL_Renderer* renderer = Common::GetRenderer();
            SDL_RenderTexture(renderer, mTextureHandle.get(), nullptr, &mPositionDimension);
            SDL_SetRenderDrawColorFloat(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
            SDL_RenderFillRect(renderer, &mPositionDimension);
            DrawChildren();
        }
    }

    void Material::DrawImguiObjectDetailsDebugMenu()
    {
        superclass::DrawImguiObjectDetailsDebugMenu();
        if(ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Image Path");
            ImGui::Indent();
            ImGui::Text(mTexturePath.empty() ? "(empty)" : mTexturePath.c_str());
            ImGui::Unindent();

            ImGui::Text("Color");
            ImGui::SameLine();
            ImGui::ColorEdit4("Color", (float*)&mColor);
        }
    }
} // namespace UI
