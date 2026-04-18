#include "UIObject.h"

#include <SDL3_ttf/SDL_ttf.h>

#include "imgui.h"

#include "Common/Data.h"
#include "Tools/Logging.h"

struct SDL_Renderer;

namespace UI
{
    static std::weak_ptr<Object> sSelectedObjectDetails;
    static bool sDetailsWindowOpen = false;

    static float sViewportWidth  = 1.0f;
    static float sViewportHeight = 1.0f;

    std::vector<ObjectSharedPtr> sRootObjects;

    bool IsRootObject(const Object* const object)
    {
        for(ObjectSharedPtr& rootObject : sRootObjects)
        {
            if(rootObject.get() == object)
            {
                return true;
            }
        }

        return false;
    }

    void Update()
    {
        SDL_Rect viewport;
        SDL_GetRenderViewport(Common::GetRenderer(), &viewport);
        sViewportWidth  = viewport.w;
        sViewportHeight = viewport.h;

        for(ObjectSharedPtr& rootObject : sRootObjects)
        {
            if(rootObject)
            {
                rootObject->Update();
            }
        }
    }

    void Draw()
    {
        for(ObjectSharedPtr& rootObject : sRootObjects)
        {
            if(rootObject)
            {
                rootObject->Draw();
            }
        }
    }

    void DrawDebug()
    {
        if(auto selectedObjectDetails = sSelectedObjectDetails.lock())
        {
            using namespace std::chrono_literals;
            constexpr static float sOneSecondInNanoseconds = (std::chrono::nanoseconds(1s)).count();

            static auto last = std::chrono::steady_clock::now();

            static float totalNanoseconds = 0;

            const auto now   = std::chrono::steady_clock::now();
            const float diff = (now - last).count();

            static int multi = 1;

            totalNanoseconds += diff * multi;
            const float ratio = totalNanoseconds / sOneSecondInNanoseconds;

            if(totalNanoseconds > sOneSecondInNanoseconds)
            {
                multi = -1;
            }
            else if(totalNanoseconds < 0)
            {
                multi = 1;
            }

            last = now;

            SDL_Renderer* renderer = Common::GetRenderer();

            SDL_SetRenderDrawColorFloat(renderer, ratio, ratio, ratio, ratio);

            SDL_RenderRect(renderer, &selectedObjectDetails->mPositionDimension);
            SDL_RenderLine(renderer, selectedObjectDetails->mPositionDimension.x,
                           selectedObjectDetails->mPositionDimension.y,
                           selectedObjectDetails->mPositionDimension.x + selectedObjectDetails->mPositionDimension.w,
                           selectedObjectDetails->mPositionDimension.y + selectedObjectDetails->mPositionDimension.h);
            SDL_RenderLine(renderer,
                           selectedObjectDetails->mPositionDimension.x + selectedObjectDetails->mPositionDimension.w,
                           selectedObjectDetails->mPositionDimension.y, selectedObjectDetails->mPositionDimension.x,
                           selectedObjectDetails->mPositionDimension.y + selectedObjectDetails->mPositionDimension.h);
        }
    }

    void Object::DrawImguiObjectDetailsDebugMenu()
    {
        if(ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Dimensions");
            ImGui::Indent();
            ImGui::Text("Width: ");
            ImGui::InputFloat("##DimensionsWidth", &mPositionDimension.w, 1.0f, 10.0f);
            ImGui::Text("Height: ");
            ImGui::InputFloat("##DimensionsHeight", &mPositionDimension.h, 1.0f, 10.0f);
            ImGui::Unindent();

            ImGui::Text("Position");
            ImGui::Indent();
            ImGui::Text((std::string("X: ") + std::to_string(mPositionDimension.x).c_str()).c_str());
            ImGui::Text((std::string("Y: ") + std::to_string(mPositionDimension.y)).c_str());
            ImGui::Unindent();

            ImGui::Text("Margin");
            ImGui::Indent();
            ImGui::Text("Left");
            ImGui::InputFloat("##DimensionsLeft", &mMargin.left, 1.0f, 10.0f);
            ImGui::Text("Top");
            ImGui::InputFloat("##DimensionsTop", &mMargin.top, 1.0f, 10.0f);
            ImGui::Text("Right");
            ImGui::InputFloat("##DimensionsRight", &mMargin.right, 1.0f, 10.0f);
            ImGui::Text("Bottom");
            ImGui::InputFloat("##DimensionsBottom", &mMargin.bottom, 1.0f, 10.0f);
            ImGui::Unindent();

            ImGui::Text("Visible");
            ImGui::Indent();
            ImGui::Checkbox("##Visible", &mVisible);
            ImGui::Unindent();

            ImGui::Text("Path");
            ImGui::Indent();
            ImGui::Text(mPath.c_str());
            ImGui::Unindent();

            ImGui::Text("Vertical Alignment");
            ImGui::Indent();
            switch(mVerticalAlignment)
            {
                case VerticalAlignment::Top:
                    ImGui::Text("Top");
                    break;
                case VerticalAlignment::Bottom:
                    ImGui::Text("Bottom");
                    break;
                case VerticalAlignment::Center:
                    ImGui::Text("Center");
                    break;
                case VerticalAlignment::Stretch:
                    ImGui::Text("Stretch");
                    break;
                default:
                    break;
            }
            ImGui::Unindent();

            ImGui::Text("Vertical Alignment");
            ImGui::Indent();
            switch(mHorizontalAlignment)
            {
                case HorizontalAlignment::Left:
                    ImGui::Text("Left");
                    break;
                case HorizontalAlignment::Right:
                    ImGui::Text("Right");
                    break;
                case HorizontalAlignment::Center:
                    ImGui::Text("Center");
                    break;
                case HorizontalAlignment::Stretch:
                    ImGui::Text("Stretch");
                    break;
                default:
                    break;
            }
            ImGui::Unindent();
        }
    }

    void Object::UpdateDimensions()
    {
        switch(mHorizontalAlignment)
        {
            case HorizontalAlignment::Left:
            case HorizontalAlignment::Right:
            case HorizontalAlignment::Center:
                break;
            case HorizontalAlignment::Stretch:
                if(mParent != nullptr)
                {
                    mPositionDimension.w = mParent->mPositionDimension.w;
                }
                else
                {
                    mPositionDimension.w = sViewportWidth;
                }
                break;
            default:
                break;
        }

        switch(mVerticalAlignment)
        {
            case VerticalAlignment::Top:
            case VerticalAlignment::Bottom:
            case VerticalAlignment::Center:
                break;
            case VerticalAlignment::Stretch:
                if(mParent != nullptr)
                {
                    mPositionDimension.h = mParent->mPositionDimension.h;
                }
                else
                {
                    mPositionDimension.h = sViewportHeight;
                }
                break;
            default:
                break;
        }
    }

    void Object::UpdatePosition()
    {
        switch(mHorizontalAlignment)
        {
            case HorizontalAlignment::Left:
            {
                float leftParentOffset = 0;
                if(mParent != 0)
                {
                    leftParentOffset = mParent->mPositionDimension.x;
                }
                mPositionDimension.x = leftParentOffset + mMargin.left;
                break;
            }
            case HorizontalAlignment::Right:
                break;
            case HorizontalAlignment::Center:
            {
                if(mParent != nullptr)
                {
                    const float parentWidthCenterX = mParent->mPositionDimension.x + mParent->mPositionDimension.w / 2;
                    mPositionDimension.x           = parentWidthCenterX - (mPositionDimension.w / 2.f);
                }
                else
                {
                    const float parentWidthCenterX = sViewportWidth / 2;
                    mPositionDimension.x           = parentWidthCenterX - (mPositionDimension.w / 2.f);
                }

                break;
            }
            case HorizontalAlignment::Stretch:
                if(mParent != nullptr)
                {
                    mPositionDimension.x = mParent->mPositionDimension.x;
                }
                else
                {
                    mPositionDimension.x = 0;
                }
                break;
            default:
                break;
        }

        switch(mVerticalAlignment)
        {
            case VerticalAlignment::Top:
            {
                float topParentOffset = 0;
                if(mParent != nullptr)
                {
                    topParentOffset = mParent->mPositionDimension.y;
                }
                mPositionDimension.y = topParentOffset + mMargin.top;
                break;
            }
            case VerticalAlignment::Bottom:
            {
                float leftParentOffset = 0;
                if(mParent != nullptr)
                {
                    leftParentOffset =
                        mParent->mPositionDimension.y + mParent->mPositionDimension.h - mPositionDimension.h;
                }
                else
                {
                    leftParentOffset = sViewportHeight - mPositionDimension.h;
                }
                mPositionDimension.y = leftParentOffset - mMargin.bottom;
                break;
            }
            case VerticalAlignment::Center:
            {
                if(mParent != nullptr)
                {
                    const float parentWidthCenterY = mParent->mPositionDimension.y + mParent->mPositionDimension.h / 2;
                    mPositionDimension.y           = parentWidthCenterY - (mPositionDimension.h / 2.f);
                }
                else
                {
                    const float parentWidthCenterY = sViewportHeight / 2;
                    mPositionDimension.y           = parentWidthCenterY - (mPositionDimension.h / 2.f);
                }

                break;
            }
            case VerticalAlignment::Stretch:
                if(mParent != nullptr)
                {
                    mPositionDimension.y = mParent->mPositionDimension.y;
                    mPositionDimension.h = mParent->mPositionDimension.h;
                }
                else
                {
                    mPositionDimension.y = 0;
                    mPositionDimension.h = sViewportHeight;
                }
                break;
            default:
                break;
        }
    }

    ObjectSharedPtr Object::FindObjectByPath(std::string_view path)
    {
        if(path == mId)
        {
            return shared_from_this();
        }

        return nullptr;
    }

    ObjectSharedPtr Object::FindObjectById(std::string_view id)
    {
        if(id == mId)
        {
            return shared_from_this();
        }

        return nullptr;
    }

    void Object::SetParent(Object& parent)
    {
        mParent = &parent;
        UpdatePath();
    }

    void Object::Initialize(const pugi::xml_node& node)
    {
        if(auto width = node.child("Width"))
        {
            mPositionDimension.w = width.text().as_float();
        }

        if(auto height = node.child("Height"))
        {
            mPositionDimension.h = height.text().as_float();
        }

        if(auto marginLeft = node.child("MarginLeft"))
        {
            mMargin.left = marginLeft.text().as_float();
        }

        if(auto marginRight = node.child("MarginRight"))
        {
            mMargin.right = marginRight.text().as_float();
        }

        if(auto marginTop = node.child("MarginTop"))
        {
            mMargin.top = marginTop.text().as_float();
        }

        if(auto marginBottom = node.child("MarginBottom"))
        {
            mMargin.bottom = marginBottom.text().as_float();
        }

        if(auto horizontalAlignment = node.child("HorizontalAlignment"))
        {
            mHorizontalAlignment = StringToHorizontalAlignmentEnum(horizontalAlignment.text().as_string());
        }

        if(auto verticalAlignment = node.child("VerticalAlignment"))
        {
            mVerticalAlignment = StringToVerticalAlignmentEnum(verticalAlignment.text().as_string());
        }
    }

    bool Object::IsInsideBounds(const float x, const float y)
    {
        auto left   = mPositionDimension.x;
        auto right  = mPositionDimension.x + mPositionDimension.w;
        auto top    = mPositionDimension.y;
        auto bottom = mPositionDimension.y + mPositionDimension.h;
        return left <= x && right >= x && top <= y && bottom >= y;
    }

    void Object::UpdatePath()
    {
        if(mParent)
        {
            mPath = mParent->mPath + "." + mId;
        }
    }

    int Object::GetImguiObjectTreeDebugNodeData(const bool forceExpand) { return ImGuiTreeNodeFlags_Leaf; }

    void Object::Update()
    {
        if(mParent == nullptr && IsRootObject(this) == false)
        {
            return;
        }

        UpdateDimensions();
        UpdatePosition();
    }

    void Object::DrawImguiObjectTreeDebugMenu(const bool forceExpand)
    {
        ImGui::PushID(mId.c_str());

        const bool isOpen = ImGui::TreeNodeEx((mId + "##" + std::to_string(mGuid.ID())).c_str(),
                                              GetImguiObjectTreeDebugNodeData(forceExpand));
        ImGui::SameLine();
        if(ImGui::SmallButton("Details"))
        {
            if(auto selectedObjectDetails = sSelectedObjectDetails.lock())
            {
                if(selectedObjectDetails.get() == this)
                {
                    sSelectedObjectDetails.reset();
                }
                else
                {
                    sSelectedObjectDetails = shared_from_this();
                }
            }
            else
            {
                sSelectedObjectDetails = shared_from_this();
            }

            sDetailsWindowOpen = sSelectedObjectDetails.expired() == false;
        }
        if(isOpen)
        {
            DrawImguiChildrenObjects(forceExpand);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }

    void RemoveAllObjects() { sRootObjects.clear(); }

    void AddRootObject(const ObjectSharedPtr& object) { sRootObjects.emplace_back(std::move(object)); }

    void DrawImguiObjectTreeDebugMenu(const bool forceExpand)
    {
        for(const ObjectSharedPtr& rootObjects : sRootObjects)
        {
            if(rootObjects)
            {
                rootObjects->DrawImguiObjectTreeDebugMenu(forceExpand);
            }
        }

        if(sDetailsWindowOpen)
        {
            if(auto selectedObjectDetails = sSelectedObjectDetails.lock())
            {
                ImGui::Begin("UI Object Details", &sDetailsWindowOpen);
                selectedObjectDetails->DrawImguiObjectDetailsDebugMenu();
                ImGui::End();
            }
        }
        else
        {
            sSelectedObjectDetails.reset();
        }
    }

    ObjectSharedPtr FindObjectByPath(std::string_view path)
    {
        for(const ObjectSharedPtr& rootObject : sRootObjects)
        {
            if(rootObject)
            {
                if(ObjectSharedPtr foundObject = rootObject->FindObjectByPath(path))
                {
                    return foundObject;
                }
            }
        }

        return nullptr;
    }

    ObjectSharedPtr FindObjectById(std::string_view id)
    {
        for(const ObjectSharedPtr& rootObject : sRootObjects)
        {
            if(rootObject)
            {
                if(ObjectSharedPtr foundObject = rootObject->FindObjectById(id))
                {
                    return foundObject;
                }
            }
        }

        return nullptr;
    }

    HorizontalAlignment StringToHorizontalAlignmentEnum(const std::string& horizontalAlignment)
    {
        if(horizontalAlignment == "Left")
        {
            return HorizontalAlignment::Left;
        }

        if(horizontalAlignment == "Right")
        {
            return HorizontalAlignment::Right;
        }

        if(horizontalAlignment == "Center")
        {
            return HorizontalAlignment::Center;
        }

        if(horizontalAlignment == "Stretch")
        {
            return HorizontalAlignment::Stretch;
        }

        return HorizontalAlignment::Stretch;
    }

    VerticalAlignment StringToVerticalAlignmentEnum(const std::string& verticalAlignment)
    {
        if(verticalAlignment == "Top")
        {
            return VerticalAlignment::Top;
        }

        if(verticalAlignment == "Bottom")
        {
            return VerticalAlignment::Bottom;
        }

        if(verticalAlignment == "Center")
        {
            return VerticalAlignment::Center;
        }

        if(verticalAlignment == "Stretch")
        {
            return VerticalAlignment::Stretch;
        }

        return VerticalAlignment::Stretch;
    }

} // namespace UI
