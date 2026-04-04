#pragma once

#include "UIObject.h"

namespace UI
{
    class Material : public Object
    {
        DECLARE_UI_ELEMENT(Material);

    public:
        void SetTexturePath(const std::string& imagePath);
        void SetColor(Color color) { mColor = color; }

        void Initialize(const pugi::xml_node& node);

    private:
        virtual void Draw() const override;

        virtual void DrawImguiObjectDetailsDebugMenu() override;

        SDLTextureUniquePtr mTextureHandle = Common::EmptyTexture();
        Color mColor                       = Common::Color::Transparent;
        std::string mTexturePath;
    };

} // namespace UI
