#include "UI/Game/PreBattleUI.h"

#include "UI/Core/UIDeserializer.h"
#include "UI/Core/UIMaterial.h"
#include "UI/Core/UIText.h"

void PreBattleUI::Init()
{
    for(UI::ObjectSharedPtr rootObject : UI::DeserializeLayout("pre_battle/pre_battle_main.xml"))
    {
        UI::AddRootObject(rootObject);
    }

    mStartButtonPanel = UI::FindObjectByPath<UI::Material>("preBattleBackground.startButtonBackground");
    mStartButtonLabel = UI::FindObjectByPath<UI::Text>("preBattleBackground.startButtonBackground.startButtonLabel");
}

void PreBattleUI::Update(const GameManager& model) {}

void PreBattleUI::OnMouseLeftButtonUp(const float mouseX, const float mouseY)
{
    if(auto startButtonPanel = mStartButtonPanel.lock())
    {
        startButtonPanel->SetColor({0, 0, 0, 1});
        if(auto startButtonLabel = mStartButtonLabel.lock())
        {
            startButtonLabel->SetColor({1, 1, 1, 1});
        }
        if(startButtonPanel->IsInsideBounds(mouseX, mouseY))
        {
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
