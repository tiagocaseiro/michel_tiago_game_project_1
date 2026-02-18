#include "GameManager.h"

GameManager::GameManager(FormationRules& formationRules, GameplayRules& gameplayRules, InitialSetup& initialSetup) : mFormationRules(formationRules), mGameplayRules(gameplayRules), mInitialSetup(initialSetup), mCardManager(initialSetup)
{
    CreateGame();    
}

void GameManager::CreateGame() 
{

}
