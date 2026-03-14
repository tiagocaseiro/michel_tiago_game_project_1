#pragma once

#include "UIObject.h"

namespace UI
{
    class Text : public Object
    {
        DECLARE_UI_ELEMENT(Text);

    public:
        void SetFontPath(const std::string& fontPath);
        void SetText(const std::string& text);
        void SetColor(Color color);
        void SetSize(int size);

    protected:
        void Update() override;

    private:
        using Object::SetHeight;
        using Object::SetWidth;

        void UpdateText();

        virtual void Draw() const override;
        virtual void DrawImguiObjectDetailsDebugMenu() override;

        Color mColor = Common::Color::BLACK;
        int mSize    = 0;

        SDLTextUniquePtr mTextHandle = Common::EmptyText();

        std::string mFontPath;
        std::string mText;
    };
} // namespace UI
