#include "UI/Game/BattleUI.h"

#include "Model/GameManager.h"
#include "UI/Core/UIDeserializer.h"
#include "UI/Core/UIMaterial.h"
#include "UI/Core/UIStackPanel.h"
#include "UI/Core/UIText.h"

static std::shared_ptr<UI::Material> MakeCardInstance(const Card& card)
{
    std::shared_ptr<UI::Material> loadedTemplateMaterial =
        UI::DeserializeObject<UI::Material>("battle/templates/battle_card_template.xml");
    if(loadedTemplateMaterial == nullptr)
    {
        return {};
    }

    loadedTemplateMaterial->SetColor(EnumUtil::ToColor(card.mColour));

    return loadedTemplateMaterial;
}

static std::shared_ptr<UI::Object> MakeFlagInstance(int /*index*/)
{
    return UI::DeserializeObject("battle/templates/flag_template.xml");
}

void BattleUI::Init()
{
    for(UI::ObjectSharedPtr rootObject : UI::DeserializeLayout("battle/battle_main.xml"))
    {
        UI::AddRootObject(rootObject);
    }

    mPlayer1HandPanel = UI::FindObjectByPath<UI::StackPanel>("handPanel0");
    mPlayer2HandPanel = UI::FindObjectByPath<UI::StackPanel>("handPanel1");
    mFlagsPanel       = UI::FindObjectByPath<UI::StackPanel>("flagsPanel");
}

void InitializeHandCards(UI::StackPanel& handPanel, const Player& player)
{
    for(const Card& card : player.mPlayerHand.mCards)
    {
        handPanel.AddChild(MakeCardInstance(card));
    }
}

void BattleUI::Update(const GameManager& model)
{
    const Board& board = model.GetBoard();

    std::shared_ptr<UI::StackPanel> flagsPanel = mFlagsPanel.lock();
    if(flagsPanel && flagsPanel->Children().empty())
    {
        InitializeFlags(board);
    }

    std::shared_ptr<UI::StackPanel> player1HandPanel = mPlayer1HandPanel.lock();
    if(player1HandPanel && player1HandPanel->Children().empty())
    {
        InitializeHandCards(*player1HandPanel, board.mPlayer1);
    }

    std::shared_ptr<UI::StackPanel> player2HandPanel = mPlayer2HandPanel.lock();
    if(player2HandPanel && player2HandPanel->Children().empty())
    {
        InitializeHandCards(*player2HandPanel, board.mPlayer2);
    }
}

void BattleUI::Shutdown() {}

void BattleUI::InitializeFlags(const Board& board)
{
    if(std::shared_ptr<UI::StackPanel> flagsPanel = mFlagsPanel.lock())
    {
        for(int i = 0; i != board.mFlags.size(); i++)
        {
            flagsPanel->AddChild(MakeFlagInstance(i));
        }
    }
}