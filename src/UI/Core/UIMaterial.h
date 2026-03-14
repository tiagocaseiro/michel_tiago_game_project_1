#pragma once

#include "UIObject.h"

namespace UI
{
    class Material : public Object
    {
        DECLARE_UI_ELEMENT(Material);

    public:
        void SetImagePath(const std::string& imagePath);
        void SetColor(Color color) { mColor = color; }

    private:
        virtual void Draw() const override;

        virtual void DrawImguiObjectDetailsDebugMenu() override;

        SDLTextureUniquePtr mTextureHandle = Common::EmptyTexture();
        Color mColor                       = Common::Color::TRANSPARENT;
        std::string mTexturePath;
    };

} // namespace UI
