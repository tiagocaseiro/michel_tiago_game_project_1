#pragma once

#include "Board.h"
#include "CardManager.h"

#include "Rules/FormationRules.h"
#include "Rules/GameplayRules.h"
#include "Rules/InitialSetup.h"

class GameManager
{
public:
    GameManager(const FormationRules& formationRules = {}, const GameplayRules& gameplayRules = {},
                const InitialSetup& initialSetup = {});

private:
    void CreateGame();

    CardManager mCardManager;
    Board mBoard;
    // #TODO: We could have the option to data-drive these
    FormationRules mFormationRules;
    GameplayRules mGameplayRules;
    InitialSetup mInitialSetup;
};