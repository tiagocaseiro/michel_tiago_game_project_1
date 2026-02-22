#include "GameManager.h"

GameManager::GameManager(const FormationRules& formationRules, const GameplayRules& gameplayRules,
                         const InitialSetup& initialSetup)
    : mFormationRules(formationRules),
      mGameplayRules(gameplayRules),
      mInitialSetup(initialSetup),
      mPlayer1(),
      mPlayer2(),
      mBoard(mPlayer1, mPlayer2, initialSetup)
{
    StartGame();
}

void GameManager::StartGame() 
{
    // #TODO_ACTIONS: distribute cards to players    
}
