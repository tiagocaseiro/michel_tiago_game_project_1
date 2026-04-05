#include "UI/Game/GameUI.h"

#include "UI/Core/UIMaterial.h"
#include "UI/Core/UIText.h"
#include "UI/Game/BattleUI.h"
#include "UI/Game/GameUI.h"
#include "UI/Game/PostBattleUI.h"
#include "UI/Game/PreBattleUI.h"

static std::unique_ptr<GameUI> sGameUI;

void GameUIManager::Init()
{
    sGameUI = std::unique_ptr<GameUI>(new PreBattleUI);
    sGameUI->Init();
}

void GameUIManager::Update(const GameManager& model)
{
    if(sGameUI != nullptr && sGameUI->IsFinished() == false)
    {
        sGameUI->Update(model);
        return;
    }

    sGameUI->Shutdown();
    UI::RemoveAllObjects();

    GameUI* currentGameUI = sGameUI.get();
    // I know this is not sophisticated but it doesn't matter at least for now
    if(dynamic_cast<PreBattleUI*>(currentGameUI) != nullptr)
    {
        sGameUI = std::unique_ptr<BattleUI>(new BattleUI);
    }
    else if(dynamic_cast<BattleUI*>(currentGameUI) != nullptr)
    {
        sGameUI = std::unique_ptr<PostBattleUI>(new PostBattleUI);
    }
    else if(dynamic_cast<PostBattleUI*>(currentGameUI) != nullptr)
    {
        sGameUI = std::unique_ptr<PreBattleUI>(new PreBattleUI);
    }

    if(sGameUI != nullptr)
    {
        sGameUI->Init();
    }
}

void GameUIManager::Shutdown()
{
    if(sGameUI != nullptr)
    {
        sGameUI->Shutdown();
    }
}
