#include "GameManager.h"

#include "Actions/DrawCardsAction.h"

GameManager::GameManager(const FormationRules& formationRules, const GameplayRules& gameplayRules,
                         const InitialSetup& initialSetup)
    : mBoard(mPlayer1, mPlayer2, initialSetup),
      mActionManager(mBoard),
      mFormationRules(formationRules),
      mGameplayRules(gameplayRules)
{
    StartGame(initialSetup);
}

void GameManager::StartGame(const InitialSetup& initialSetup)
{
    DistributeInitialCards(initialSetup, mPlayer1);
    DistributeInitialCards(initialSetup, mPlayer2);
}
void GameManager::DistributeInitialCards(const InitialSetup& initialSetup, Player& player)
{
    auto actionParams = std::make_unique<DrawCardsActionParams>(mBoard.mTroopDeck, player, initialSetup.mInitialCardsPerPlayer);
    auto actionInstance = std::make_unique<DrawCardsAction>(mBoard, std::move(actionParams));
    mActionManager.AddAction(std::move(actionInstance), /*immediateApply = */true);
}
