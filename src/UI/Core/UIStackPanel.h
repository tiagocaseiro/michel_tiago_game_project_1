#pragma once

#include "UIPanel.h"

namespace UI
{
    class StackPanel : public Panel
    {
        DECLARE_UI_ELEMENT_DERIVED(StackPanel, Panel);

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

    private:
        void VerticalUpdate();
        void HorizontalUpdate();

        float mPadding           = 0;
        Orientation mOrientation = Orientation::Horizontal;
    };
} // namespace UI
