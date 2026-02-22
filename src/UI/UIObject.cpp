#include "UIObject.h"

#include <chrono>

#include "imgui.h"

#include "Tools/Logging.h"

namespace UI
{
    static SDL_Renderer* sRenderer              = nullptr;
    static const Object* sSelectedObjectDetails = {};
    static bool sDetailsWindowOpen              = false;

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

            SDL_SetRenderDrawColorFloat(sRenderer, ratio, ratio, ratio, ratio);
            SDL_RenderRect(sRenderer, &sSelectedObjectDetails->mPositionDimension);
            SDL_RenderLine(sRenderer, sSelectedObjectDetails->mPositionDimension.x,
                           sSelectedObjectDetails->mPositionDimension.y,
                           sSelectedObjectDetails->mPositionDimension.x + sSelectedObjectDetails->mPositionDimension.w,
                           sSelectedObjectDetails->mPositionDimension.y + sSelectedObjectDetails->mPositionDimension.h);
            SDL_RenderLine(sRenderer,
                           sSelectedObjectDetails->mPositionDimension.x + sSelectedObjectDetails->mPositionDimension.w,
                           sSelectedObjectDetails->mPositionDimension.y, sSelectedObjectDetails->mPositionDimension.x,
                           sSelectedObjectDetails->mPositionDimension.y + sSelectedObjectDetails->mPositionDimension.h);
        }
    }

    void SetRenderer(SDL_Renderer* renderer) { sRenderer = renderer; }

    Object::~Object()
    {
        if(sSelectedObjectDetails == this)
        {
            sSelectedObjectDetails = nullptr;
            sDetailsWindowOpen     = false;
        }
    }

    void Object::DrawImguiObjectDetailsDebugMenu() const
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

        ImGui::Text("Color");
        ImGui::SameLine();
        ImGui::ColorButton("Color", (ImVec4&)mColor);

        ImGui::Text("Margin");
        ImGui::Indent();
        ImGui::Text((std::string("left ") + std::to_string(mMargin.left)).c_str());
        ImGui::Text((std::string("top ") + std::to_string(mMargin.top)).c_str());
        ImGui::Text((std::string("roght ") + std::to_string(mMargin.right)).c_str());
        ImGui::Text((std::string("bottom ") + std::to_string(mMargin.bottom)).c_str());
        ImGui::Unindent();
    }

    void Object::AddChild(ObjectPtr object)
    {
        if(object == nullptr)
        {
            Logging::LogWarning("Attempted to attach an empty UI Object child");
            return;
        }
        object->SetParent(this);
        mChildren.emplace_back(std::move(object));
    }

    void Object::Draw() const
    {
        SDL_SetRenderDrawColorFloat(sRenderer, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderFillRect(sRenderer, &mPositionDimension);

        for(const ObjectPtr& child : mChildren)
        {
            child->Draw();
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

    void Object::DrawImguiObjectTreeDebugMenu() const
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

    void RootObject::DrawImguiObjectTreeDebugMenu() const
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

    Image::~Image() { SDL_DestroyTexture(mTexture); }

    void Image::SetImagePath(const std::string& imagePath)
    {
        mImagePath              = imagePath;
        SDL_Surface* surface    = SDL_LoadSurface(imagePath.c_str());
        SDL_Texture* newTexture = SDL_CreateTextureFromSurface(sRenderer, surface);

        SDL_DestroySurface(surface);
        SDL_DestroyTexture(mTexture);

        mTexture = newTexture;
    }

    void Image::Draw() const
    {
        SDL_RenderTexture(sRenderer, mTexture, nullptr, &mPositionDimension);
        superclass::Draw();
    }

    void Image::DrawImguiObjectDetailsDebugMenu() const
    {
        superclass::DrawImguiObjectDetailsDebugMenu();
        ImGui::Text("Image Path");
        ImGui::Indent();
        ImGui::Text(mImagePath.c_str());
        ImGui::Unindent();
    }

    RootObject& Root()
    {
        static RootObject root = RootObject("root");
        return root;
    }

    void Text::Draw() const {}

} // namespace UI
