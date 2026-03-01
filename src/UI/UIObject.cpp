#include "UIObject.h"

#include "imgui.h"

#include "Common/Data.h"
#include "Common/Fonts.h"
#include "Tools/Logging.h"

struct SDL_Renderer;

namespace UI
{
    static Object* sSelectedObjectDetails = nullptr;
    static bool sDetailsWindowOpen        = false;

    void Update() { Root().Update(); }

    void Draw() { Root().Draw(); }

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
            ImGui::Text((std::string("width: ") + std::to_string(mPositionDimension.w)).c_str());
            ImGui::Text((std::string("height: ") + std::to_string(mPositionDimension.h)).c_str());
            ImGui::Unindent();

            ImGui::Text("Position");
            ImGui::Indent();
            ImGui::Text((std::string("x: ") + std::to_string(mPositionDimension.x).c_str()).c_str());
            ImGui::Text((std::string("y: ") + std::to_string(mPositionDimension.y)).c_str());
            ImGui::Unindent();

            ImGui::Text("Margin");
            ImGui::Indent();
            ImGui::Text((std::string("left ") + std::to_string(mMargin.left)).c_str());
            ImGui::Text((std::string("top ") + std::to_string(mMargin.top)).c_str());
            ImGui::Text((std::string("roght ") + std::to_string(mMargin.right)).c_str());
            ImGui::Text((std::string("bottom ") + std::to_string(mMargin.bottom)).c_str());
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
        if(mParent == nullptr)
        {
            return;
        }

        // Defaulting to LEFT alignment
        mPositionDimension.x = mParent->mPositionDimension.x + mMargin.left;
        mPositionDimension.y = mParent->mPositionDimension.y + mMargin.top;

        for(const ObjectPtr& child : mChildren)
        {
            child->Update();
        }
    }

    void Object::DrawImguiObjectTreeDebugMenu()
    {
        ImGui::PushID(mId.c_str());

        const bool isOpen =
            ImGui::TreeNodeEx(mId.c_str(), mChildren.empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None);
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
                child->DrawImguiObjectTreeDebugMenu();
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }

    void RootObject::Draw() const
    {
        for(const ObjectPtr& child : mChildren)
        {
            if(child)
            {
                child->Draw();
            }
        }
    }
    void RootObject::Update()
    {
        for(const ObjectPtr& child : mChildren)
        {
            if(child)
            {
                child->Update();
            }
        }
    }

    void RootObject::DrawImguiObjectTreeDebugMenu()
    {
        if(ImGui::TreeNode(mId.c_str()))
        {
            for(const ObjectPtr& child : mChildren)
            {
                if(child)
                {
                    child->DrawImguiObjectTreeDebugMenu();
                }
            }
            ImGui::TreePop();
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
        if(mVisibility && mParent->GetVisibility())
        {
            SDL_Renderer* renderer = Common::GetRenderer();
            SDL_RenderTexture(renderer, mTexture.get(), nullptr, &mPositionDimension);
            SDL_SetRenderDrawColorFloat(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
            SDL_RenderFillRect(renderer, &mPositionDimension);
        }

        DrawChildren();
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
            ImGui::ColorButton("Color", (ImVec4&)mColor);
        }
    }

    RootObject& Root()
    {
        static RootObject root = RootObject("root");
        return root;
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
        if(mText != text)
        {
            mText = text;
            if(mFont)
            {
                mTexture.release();
                mTexture = Common::LoadTextTexture(*mFont, mColor, mText);
            }
            else
            {
                Logging::LogWarning("Attempted to initialize text but font hasn't been initialized.");
            }
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
            ImGui::Text(mText.c_str());
            ImGui::Unindent();

            ImGui::Text("Color");
            ImGui::SameLine();
            ImGui::ColorButton("Color", (ImVec4&)mColor);

            ImGui::Text("Font");
            ImGui::Indent();
            ImGui::Text(mFontPath.c_str());
            ImGui::Unindent();
        }
    }

} // namespace UI
