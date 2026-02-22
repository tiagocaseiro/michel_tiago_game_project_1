#include "GameManager.h"

GameManager::GameManager(const FormationRules& formationRules, const GameplayRules& gameplayRules,
                         const InitialSetup& initialSetup)
    : mFormationRules(formationRules),
      mGameplayRules(gameplayRules),
      mInitialSetup(initialSetup),
      mCardManager(initialSetup)
{
    CreateGame();
}

void GameManager::CreateGame() {}
