#include "UIStackPanel.h"

#include <pugixml.hpp>

#include "imgui.h"

namespace UI
{
    void StackPanel::Initialize(const pugi::xml_node& node)
    {
        superclass::Initialize(node);
        mPadding = node.child("Padding").text().as_int();

        const std::string orientation = node.child("Orientation").text().as_string();
        if(orientation == "Horizontal")
        {
            mOrientation = Orientation::Horizontal;
        }
        else if(orientation == "Vertical")
        {
            mOrientation = Orientation::Vertical;
        }
    }

    void StackPanel::Update()
    {
        superclass::Update();

        switch(mOrientation)
        {
            case Orientation::Vertical:
                VerticalUpdate();
                break;
            case Orientation::Horizontal:
                HorizontalUpdate();
                break;
            default:
                break;
        }
    }

    void StackPanel::Draw() const
    {
        for(ObjectSharedPtr child : mChildren)
        {
            if(child)
            {
                child->Draw();
            }
        }
    }

    void StackPanel::VerticalUpdate() {}

    void StackPanel::HorizontalUpdate()
    {
        float maxChildHeight = 0;
        float startX         = mPositionDimension.x;
        for(int i = 0; i != mChildren.size(); i++)
        {
            ObjectSharedPtr child = mChildren[i];
            if(i != 0)
            {
                startX += mPadding;
            }

            if(child)
            {
                child->mPositionDimension.x = startX;
                startX += child->mPositionDimension.w;
                maxChildHeight = std::max(maxChildHeight, child->mPositionDimension.h);
            }
        }
        mPositionDimension.h = maxChildHeight;
        mPositionDimension.w = startX - mPositionDimension.x;
    }
} // namespace UI
