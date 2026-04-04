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

        void Initialize(const pugi::xml_node& node);

    protected:
        void Update() override;

    private:
        using Object::SetHeight;
        using Object::SetWidth;

        void UpdateText();

        virtual void Draw() const override;
        virtual void DrawImguiObjectDetailsDebugMenu() override;

        SDLTextUniquePtr mTextHandle = Common::EmptyText();

        Color mColor = Common::Color::Black;
        int mSize    = 0;

        std::string mFontPath;
        std::string mText;
    };
} // namespace UI
