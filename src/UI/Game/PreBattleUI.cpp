#include "UI/Game/PreBattleUI.h"

#include "UI/Core/UIMaterial.h"
#include "UI/Core/UIText.h"

void PreBattleUI::Init()
{
    std::shared_ptr<UI::Text> gameTitle = UI::Text::Make("gameTitle");
    gameTitle->SetFontPath("fonts/canterbury.ttf");
    gameTitle->SetColor({0, 0, 0, 1});
    gameTitle->SetHorizontalAlignment(UI::HorizontalAlignment::Center);
    gameTitle->SetVerticalAlignment(UI::VerticalAlignment::Top);
    gameTitle->SetTopMargin(300);
    gameTitle->SetText("Battle Line");
    gameTitle->SetSize(150);

    std::shared_ptr<UI::Material> startButtonParent = UI::Material::Make("startButtonParent");
    startButtonParent->SetColor({0, 0, 0, 1});
    startButtonParent->SetTopMargin(500);
    startButtonParent->SetWidth(200);
    startButtonParent->SetHeight(60);
    startButtonParent->SetHorizontalAlignment(UI::HorizontalAlignment::Center);
    startButtonParent->SetVerticalAlignment(UI::VerticalAlignment::Top);

    std::shared_ptr<UI::Text> startButtonLabel = UI::Text::Make("startButtonLabel");
    startButtonLabel->SetFontPath("fonts/montserrat.ttf");
    startButtonLabel->SetColor({1, 1, 1, 1});
    startButtonLabel->SetHorizontalAlignment(UI::HorizontalAlignment::Center);
    startButtonLabel->SetVerticalAlignment(UI::VerticalAlignment::Center);
    startButtonLabel->SetText("Start Game");
    startButtonLabel->SetSize(30);

    startButtonParent->AddChild(startButtonLabel);

    mStartButtonPanel = startButtonParent;
    mStartButtonLabel = startButtonLabel;

    UI::AddObject(gameTitle);
    UI::AddObject(startButtonParent);
}

void PreBattleUI::Update() {}

void PreBattleUI::OnMouseLeftButtonUp(const float mouseX, const float mouseY)
{
    if(auto startButtonPanel = mStartButtonPanel.lock())
    {
        startButtonPanel->SetColor({0, 0, 0, 1});
        if(auto startButtonLabel = mStartButtonLabel.lock())
        {
            startButtonLabel->SetColor({1, 1, 1, 1});
            SetAsFinished();
        }
    }
}

void PreBattleUI::OnMouseLeftButtonDown(const float mouseX, const float mouseY)
{
    if(auto startButtonPanel = mStartButtonPanel.lock())
    {
        if(startButtonPanel->IsInsideBounds(mouseX, mouseY))
        {
            startButtonPanel->SetColor({1, 1, 1, 1});
            if(auto startButtonLabel = mStartButtonLabel.lock())
            {
                startButtonLabel->SetColor({0, 0, 0, 1});
            }
        }
    }
}

void PreBattleUI::Shutdown() { UI::RemoveAllObjects(); }
