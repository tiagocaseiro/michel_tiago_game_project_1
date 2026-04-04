#include "UIObject.h"

#include <algorithm>

#include <SDL3_ttf/SDL_ttf.h>

#include "imgui.h"

#include "Common/Data.h"
#include "Tools/Logging.h"
#include "UI/Core/UIDeserializer.h"

struct SDL_Renderer;

namespace UI
{
    static constexpr auto sPathSeparator = '.';

    static Object* sSelectedObjectDetails = nullptr;
    static bool sDetailsWindowOpen        = false;

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
        if(sSelectedObjectDetails)
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
            SDL_RenderRect(renderer, &sSelectedObjectDetails->mPositionDimension);
            SDL_RenderLine(renderer, sSelectedObjectDetails->mPositionDimension.x,
                           sSelectedObjectDetails->mPositionDimension.y,
                           sSelectedObjectDetails->mPositionDimension.x + sSelectedObjectDetails->mPositionDimension.w,
                           sSelectedObjectDetails->mPositionDimension.y + sSelectedObjectDetails->mPositionDimension.h);
            SDL_RenderLine(renderer,
                           sSelectedObjectDetails->mPositionDimension.x + sSelectedObjectDetails->mPositionDimension.w,
                           sSelectedObjectDetails->mPositionDimension.y, sSelectedObjectDetails->mPositionDimension.x,
                           sSelectedObjectDetails->mPositionDimension.y + sSelectedObjectDetails->mPositionDimension.h);
        }
    }

    Object::~Object()
    {
        if(sSelectedObjectDetails == this)
        {
            sSelectedObjectDetails = nullptr;
            sDetailsWindowOpen     = false;
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

    void Object::AddChild(const ObjectSharedPtr& object)
    {
        if(object == nullptr)
        {
            Logging::LogWarning("Attempted to attach an empty UI Object child");
            return;
        }
        object->SetParent(*this);
        mChildren.emplace_back(object);
    }

    void Object::RemoveChild(std::string_view childId)
    {
        const auto found = std::ranges::find_if(mChildren, [childId](const auto& child) {
            return child && child->mId == childId;
        });

        if(found == std::end(mChildren))
        {
            Logging::LogWarning("Failed to find child object with id {} in object {}", childId, mPath);
            return;
        }

        mChildren.erase(found);
    }

    void Object::RemoveChild(int childIndex)
    {
        if(childIndex >= mChildren.size())
        {
            Logging::LogWarning("Attempted to remove child with index {} beyond size of {} in object {}", childIndex,
                                mChildren.size(), mPath);
            return;
        }

        auto it = std::begin(mChildren);

        std::advance(it, childIndex);
        mChildren.erase(it);
    }

    void Object::RemoveAllChildren() { mChildren.clear(); }

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
                    leftParentOffset = mParent->mPositionDimension.y;
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

    Object* Object::FindObjectByPath(std::string_view path)
    {
        if(path == mId)
        {
            return this;
        }
        // This can be more efficient by storing a path object
        // It's hella heavy to do all this string manipulation every time we search a path
        // Also we don't want to depend on '.'
        const auto frontSeparatorIndex = path.find_first_of(sPathSeparator);
        const auto frontId             = path.substr(0, frontSeparatorIndex);

        if(frontId == mId)
        {
            const auto rest = path.substr(frontSeparatorIndex + 1);

            for(const ObjectSharedPtr& object : mChildren)
            {
                if(object)
                {
                    if(Object* foundObject = object->FindObjectByPath(rest))
                    {
                        return foundObject;
                    }
                }
            }
        }

        return nullptr;
    }

    void Object::DrawChildren() const
    {
        for(const ObjectSharedPtr& child : mChildren)
        {
            child->Draw();
        }
    }

    void Object::SetParent(Object& parent)
    {
        mParent = &parent;
        UpdatePath();
    }

    void Object::Initialize(const pugi::xml_node& node)
    {
        assert(true, "Add if node checks");

        mPositionDimension.w = node.child("Width").text().as_float();
        mPositionDimension.h = node.child("Height").text().as_float();
        mMargin.left         = node.child("MarginLeft").text().as_float();
        mMargin.right        = node.child("MarginRight").text().as_float();
        mMargin.top          = node.child("MarginTop").text().as_float();
        mMargin.bottom       = node.child("MarginBottom").text().as_float();

        const std::string horizontalAlignment = node.child("HorizontalAlignment").text().as_string();
        if(horizontalAlignment == "Left")
        {
            mHorizontalAlignment = HorizontalAlignment::Left;
        }
        else if(horizontalAlignment == "Right")
        {
            mHorizontalAlignment = HorizontalAlignment::Right;
        }
        else if(horizontalAlignment == "Center")
        {
            mHorizontalAlignment = HorizontalAlignment::Center;
        }
        else if(horizontalAlignment == "Stretch")
        {
            mHorizontalAlignment = HorizontalAlignment::Stretch;
        }

        const std::string verticalAlignment = node.child("VerticalAlignment").text().as_string();
        if(verticalAlignment == "Left")
        {
            mVerticalAlignment = VerticalAlignment::Top;
        }
        else if(verticalAlignment == "Right")
        {
            mVerticalAlignment = VerticalAlignment::Bottom;
        }
        else if(verticalAlignment == "Center")
        {
            mVerticalAlignment = VerticalAlignment::Center;
        }
        else if(verticalAlignment == "Stretch")
        {
            mVerticalAlignment = VerticalAlignment::Stretch;
        }

        auto children = node.child("Children");
        for(auto it = children.begin(); it != children.end(); it++)
        {
            AddChild(DeserializeNode(*it));
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
            mPath = mParent->mPath + "." + mPath;
        }

        for(auto& children : mChildren)
        {
            if(children)
            {
                children->UpdatePath();
            }
        }
    }

    void Object::Update()
    {
        if(mParent == nullptr && IsRootObject(this) == false)
        {
            return;
        }

        UpdateDimensions();
        UpdatePosition();

        for(const ObjectSharedPtr& child : mChildren)
        {
            child->Update();
        }
    }

    void Object::DrawImguiObjectTreeDebugMenu(const bool forceExpand)
    {
        ImGui::PushID(mId.c_str());

        auto treeNodeFlags = ImGuiTreeNodeFlags_None;
        if(mChildren.empty())
        {
            treeNodeFlags = ImGuiTreeNodeFlags_Leaf;
        }
        else if(forceExpand)
        {
            treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
        }

        const bool isOpen = ImGui::TreeNodeEx(mId.c_str(), treeNodeFlags);
        ImGui::SameLine();
        if(ImGui::SmallButton("Details"))
        {
            if(sSelectedObjectDetails != nullptr)
            {
                if(sSelectedObjectDetails == this)
                {
                    sSelectedObjectDetails = nullptr;
                }
                else
                {
                    sSelectedObjectDetails = this;
                }
            }
            else
            {
                sSelectedObjectDetails = this;
            }

            sDetailsWindowOpen = sSelectedObjectDetails != nullptr;
        }
        if(isOpen)
        {
            for(const ObjectSharedPtr& child : mChildren)
            {
                child->DrawImguiObjectTreeDebugMenu(forceExpand);
            }
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
            if(sSelectedObjectDetails != nullptr)
            {
                ImGui::Begin("UI Object Details", &sDetailsWindowOpen);
                sSelectedObjectDetails->DrawImguiObjectDetailsDebugMenu();
                ImGui::End();
            }
        }
        else
        {
            sSelectedObjectDetails = nullptr;
        }
    }

    Object* FindObjectByPath(std::string_view path)
    {
        for(const ObjectSharedPtr& rootObject : sRootObjects)
        {
            if(rootObject)
            {
                if(Object* foundObject = rootObject->FindObjectByPath(path))
                {
                    return foundObject;
                }
            }
        }

        return nullptr;
    }
} // namespace UI
