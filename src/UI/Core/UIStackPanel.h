#pragma once

#include "UIObject.h"

namespace UI
{
    class StackPanel : public Object
    {
        DECLARE_UI_ELEMENT(StackPanel);

    public:
        enum class Orientation
        {
            Vertical,
            Horizontal
        };
        void SetPadding(int padding) { mPadding = padding; }
        void SetOrientation(Orientation orientation) { mOrientation = orientation; }

        void Initialize(const pugi::xml_node& node);

    protected:
        void Update() override;
        void Draw() const override;

    private:
        void VerticalUpdate();
        void HorizontalUpdate();

        float mPadding           = 0;
        Orientation mOrientation = Orientation::Horizontal;
    };
} // namespace UI
