#include "UI/Game/BattleUI.h"

#include "Model/GameManager.h"
#include "UI/Core/UIDeserializer.h"
#include "UI/Core/UIMaterial.h"
#include "UI/Core/UIStackPanel.h"
#include "UI/Core/UIText.h"

static std::shared_ptr<UI::Material> MakeCardInstance(const Card& card)
{
    std::shared_ptr<UI::Material> cardInstance =
        UI::Material::Make("cardParent" + EnumUtil::ToString(card.mColour) + std::to_string(card.mPower));

    cardInstance->SetWidth(50);
    cardInstance->SetHeight(100);
    cardInstance->SetColor(EnumUtil::ToColor(card.mColour));

    return cardInstance;
}

static std::shared_ptr<UI::Material> MakeFlagInstance(int index)
{
    std::shared_ptr<UI::Material> flagInstance = UI::Material::Make("flag" + std::to_string(index));

    flagInstance->SetColor({1, 0, 0, 1});
    flagInstance->SetTopMargin(500);
    flagInstance->SetWidth(50);
    flagInstance->SetHeight(50);
    flagInstance->SetVerticalAlignment(UI::VerticalAlignment::Center);

    return flagInstance;
}

void BattleUI::Init()
{
    for(UI::ObjectSharedPtr rootObject : UI::DeserializeLayout("ui/battle.xml"))
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