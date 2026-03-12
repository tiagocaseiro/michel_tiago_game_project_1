#include "UIObject.h"

#include "imgui.h"

#include "Common/Data.h"
#include "Common/Fonts.h"
#include "Tools/Logging.h"

struct SDL_Renderer;

namespace UI
{
    static constexpr auto sPathSeparator = '.';

    static Object* sSelectedObjectDetails = nullptr;
    static bool sDetailsWindowOpen        = false;

    std::vector<ObjectPtr> sObjects;

    void Update()
    {
        for(ObjectPtr& object : sObjects)
        {
            if(object)
            {
                object->Update();
            }
        }
    }

    void Draw()
    {
        for(ObjectPtr& object : sObjects)
        {
            if(object)
            {
                object->Draw();
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

            ImGui::Text("Visibility");
            ImGui::Indent();
            ImGui::Checkbox("##Visible", &mVisibility);
            ImGui::Unindent();

            ImGui::Text("Path");
            ImGui::Indent();
            ImGui::Text(mPath.c_str());
            ImGui::Unindent();
        }
    }

    void Object::AddChild(ObjectPtr object)
    {
        if(object == nullptr)
        {
            Logging::LogWarning("Attempted to attach an empty UI Object child");
            return;
        }
        object->SetParent(*this);
        mChildren.emplace_back(std::move(object));
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

            for(const ObjectPtr& object : mChildren)
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
        for(const ObjectPtr& child : mChildren)
        {
            child->Draw();
        }
    }

    void Object::SetParent(Object& parent)
    {
        mParent = &parent;
        UpdatePath();
    }

    void Object::UpdatePath()
    {
        mPath = mId;
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
        const auto rootObjectIt = std::ranges::find_if(sObjects, [this](const ObjectPtr& object) {
            return object.get() == this;
        });

        const bool isRootObject = (rootObjectIt != std::end(sObjects));

        if(mParent == nullptr && isRootObject == false)
        {
            return;
        }

        if(isRootObject)
        {
            mPositionDimension.x = mMargin.left;
            mPositionDimension.y = mMargin.top;
        }
        else
        {
            // Defaulting to LEFT alignment
            mPositionDimension.x = mParent->mPositionDimension.x + mMargin.left;
            mPositionDimension.y = mParent->mPositionDimension.y + mMargin.top;
        }

        for(const ObjectPtr& child : mChildren)
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
            for(const ObjectPtr& child : mChildren)
            {
                child->DrawImguiObjectTreeDebugMenu(forceExpand);
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }

    void Material::SetImagePath(const std::string& texturePath)
    {
        if(mTexturePath != texturePath)
        {
            mTexturePath = texturePath;
            mTexture     = Common::LoadTexture(mTexturePath);
        }
    }

    void Material::Draw() const
    {
        if(mVisibility)
        {
            SDL_Renderer* renderer = Common::GetRenderer();
            SDL_RenderTexture(renderer, mTexture.get(), nullptr, &mPositionDimension);
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

    void Text::SetFontPath(const std::string& fontPath)
    {
        if(mFontPath != fontPath)
        {
            mFontPath = fontPath;
            mFont     = Common::TryGetFont(mFontPath);
        }
    }

    void Text::SetText(const std::string& text)
    {
        mText = text;
        UpdateTexture();
    }

    void Text::SetColor(Color color)
    {
        mColor = color;
        UpdateTexture();
    }

    void Text::UpdateTexture()
    {
        if(mFont == nullptr)
        {
            Logging::LogWarning("Attempted to initialize text but font hasn't been initialized.");
            return;
        }

        if(mText.empty() == false)
        {
            mTexture = Common::LoadTextTexture(*mFont, mColor, mText);
        }
    }

    void Text::Draw() const { SDL_RenderTexture(Common::GetRenderer(), mTexture.get(), nullptr, &mPositionDimension); }

    void Text::DrawImguiObjectDetailsDebugMenu()
    {
        superclass::DrawImguiObjectDetailsDebugMenu();
        if(ImGui::CollapsingHeader("Text", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Text");
            ImGui::Indent();

            static char buffer[512] = {0};

            std::strcpy(buffer, mText.data());

            if(ImGui::InputText("##TextInput", buffer, 512))
            {
                SetText(buffer);
            }
            ImGui::Unindent();

            ImGui::Text("Color");
            ImGui::SameLine();
            if(ImGui::ColorEdit4("Color", (float*)&mColor))
            {
                UpdateTexture();
            }

            ImGui::Text("Font");
            ImGui::Indent();
            ImGui::Text(mFontPath.c_str());
            ImGui::Unindent();
        }
    }

    void RemoveAllObjects() { sObjects.clear(); }

    void AddObject(ObjectPtr&& object) { sObjects.emplace_back(std::move(object)); }

    void DrawImguiObjectTreeDebugMenu(const bool forceExpand)
    {
        for(const ObjectPtr& child : sObjects)
        {
            if(child)
            {
                child->DrawImguiObjectTreeDebugMenu(forceExpand);
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
        for(const ObjectPtr& object : sObjects)
        {
            if(object)
            {
                if(Object* foundObject = object->FindObjectByPath(path))
                {
                    return foundObject;
                }
            }
        }

        return nullptr;
    }

} // namespace UI
