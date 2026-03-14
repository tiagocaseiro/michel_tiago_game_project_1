#include "UI/Game/GameUI.h"

#include "GameUI.h"
#include "UI/Core/UIMaterial.h"
#include "UI/Core/UIText.h"

GameUI& GameUI::Instance()
{
    static GameUI gameUI;
    return gameUI;
}

GameUI::GameUI() : mState(State::Invalid) {}

void GameUI::EnterState(State const state)
{
    ExitState(mState);

    mState = state;

    switch(mState)
    {
        case State::PreBattle:
            EnterPreBattleState();
            break;
        case State::Battle:
            EnterBattleState();
            break;
        case State::PostBattle:
            EnterPostBattleState();
            break;
        default:
            break;
    }
}

void GameUI::ExitState(State const state)
{

    switch(mState)
    {
        case State::PreBattle:
            ExitPreBattleState();
            break;
        case State::Battle:
            ExitBattleState();
            break;
        case State::PostBattle:
            ExitPostBattleState();
            break;

        default:
            break;
    }
}

void GameUI::Init() { EnterState(State::PreBattle); }

void GameUI::Update() {}
void GameUI::EnterPreBattleState()
{
    UI::RemoveAllObjects();

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

void GameUI::EnterBattleState() {}

void GameUI::EnterPostBattleState() {}

void GameUI::UpdatePreBattleState() {}

void GameUI::UpdateBattleState() {}

void GameUI::UpdatePostBattleState() {}

void GameUI::ExitPreBattleState() {}

void GameUI::ExitBattleState() {}

void GameUI::ExitPostBattleState() {}

void GameUI::OnMouseLeftButtonUp(const float mouseX, const float mouseY)
{
    if(auto startButtonPanel = mStartButtonPanel.lock())
    {
        startButtonPanel->SetColor({0, 0, 0, 1});
        if(auto startButtonLabel = mStartButtonLabel.lock())
        {
            startButtonLabel->SetColor({1, 1, 1, 1});
        }
    }
}

void GameUI::OnMouseLeftButtonDown(const float mouseX, const float mouseY)
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

void GameUI::Shutdown() { UI::RemoveAllObjects(); }
