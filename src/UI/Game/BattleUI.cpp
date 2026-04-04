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

static std::shared_ptr<UI::StackPanel> MakeHandPanel(int playerIndex)
{
    std::shared_ptr<UI::StackPanel> handPanel = UI::StackPanel::Make("handPanel" + std::to_string(playerIndex));

    handPanel->SetOrientation(UI::StackPanel::Orientation::Horizontal);
    handPanel->SetPadding(20);
    handPanel->SetVerticalAlignment(playerIndex == 0 ? UI::VerticalAlignment::Top : UI::VerticalAlignment::Bottom);
    handPanel->SetHorizontalAlignment(UI::HorizontalAlignment::Center);

    return handPanel;
}

static std::shared_ptr<UI::StackPanel> MakeFlagPanel()
{
    std::shared_ptr<UI::StackPanel> flagsPanel = UI::StackPanel::Make("flagsPanel");

    flagsPanel->SetPadding(10);
    flagsPanel->SetVerticalAlignment(UI::VerticalAlignment::Center);
    flagsPanel->SetHorizontalAlignment(UI::HorizontalAlignment::Center);

    return flagsPanel;
}

void BattleUI::Init()
{
    UI::DeserializeLayout("ui/button.ui_layout");
    mPlayer1HandPanel = MakeHandPanel(0);
    mPlayer2HandPanel = MakeHandPanel(1);

    UI::AddRootObject(mPlayer1HandPanel);
    UI::AddRootObject(mPlayer2HandPanel);

    mFlagsPanel = MakeFlagPanel();
    UI::AddRootObject(mFlagsPanel);
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

    if(mFlagsPanel && mFlagsPanel->Children().empty())
    {
        InitializeFlags(board);
    }

    if(mPlayer1HandPanel && mPlayer1HandPanel->Children().empty())
    {
        InitializeHandCards(*mPlayer1HandPanel, board.mPlayer1);
    }

    if(mPlayer2HandPanel && mPlayer2HandPanel->Children().empty())
    {
        InitializeHandCards(*mPlayer2HandPanel, board.mPlayer2);
    }
}

void BattleUI::Shutdown() {}

void BattleUI::InitializeFlags(const Board& board)
{
    if(mFlagsPanel)
    {
        for(int i = 0; i != board.mFlags.size(); i++)
        {
            mFlagsPanel->AddChild(MakeFlagInstance(i));
        }
    }
}