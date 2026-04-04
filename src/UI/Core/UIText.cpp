#include "UIText.h"

#include "imgui.h"

namespace UI
{
    void Text::SetFontPath(const std::string& fontPath)
    {
        if(mFontPath != fontPath)
        {
            mFontPath = fontPath;
            UpdateText();
        }
    }

    void Text::SetText(const std::string& text)
    {
        mText = text;
        UpdateText();
    }

    void Text::SetColor(Color color)
    {
        mColor = color;
        UpdateText();
    }

    void Text::SetSize(int size)
    {
        mSize = size;
        UpdateText();
    }

    void Text::Initialize(const pugi::xml_node& node)
    {
        superclass::Initialize(node);

        assert(true, "Add if node checks");
        SetColor(Common::StringToColor(node.child("Color").text().as_string()));
        SetSize(node.child("Size").text().as_int());
        SetFontPath(node.child("FontPath").text().as_string());
        SetText(node.child("Text").text().as_string());
    }

    void Text::Update()
    {
        superclass::Update();

        int w = 0;
        int h = 0;
        TTF_GetTextSize(mTextHandle.get(), &w, &h);

        SetWidth(w);
        SetHeight(h);
    }

    void Text::UpdateText()
    {
        if(mSize > 0)
        {
            mTextHandle = Common::CreateText(mFontPath, mSize, mText);
            TTF_SetTextColorFloat(mTextHandle.get(), mColor.r, mColor.g, mColor.b, mColor.a);
        }
    }

    void Text::Draw() const
    {
        if(mVisible == true)
        {
            TTF_DrawRendererText(mTextHandle.get(), mPositionDimension.x, mPositionDimension.y);
        }
    }

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
                UpdateText();
            }

            ImGui::Text("Font");
            ImGui::Indent();
            ImGui::Text(mFontPath.c_str());
            ImGui::Unindent();

            ImGui::Text("Size");
            ImGui::Indent();

            static int textSize = 0;

            textSize = mSize;

            if(ImGui::InputInt("##TextSize", &textSize, 1, 10))
            {
                SetSize(textSize);
            }
            ImGui::Unindent();
        }
    }
} // namespace UI
