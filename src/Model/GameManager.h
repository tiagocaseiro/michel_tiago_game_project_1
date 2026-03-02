#pragma once

#include "Board.h"
#include "CardManager.h"
#include "Player.h"

#include "Rules/FormationRules.h"
#include "Rules/GameplayRules.h"
#include "Rules/InitialSetup.h"
#include "Actions/ActionManager.h"

class GameManager
{
public:
    GameManager(const FormationRules& formationRules = {}, const GameplayRules& gameplayRules = {},
                const InitialSetup& initialSetup = {});

private:
    void StartGame(const InitialSetup& initialSetup);
    void DistributeInitialCards(const InitialSetup& initialSetup, Player& player);

    Player mPlayer1;
    Player mPlayer2;
    Board mBoard;
    ActionManager mActionManager;

    // #TODO: We could have the option to data-drive these
    FormationRules mFormationRules;
    GameplayRules mGameplayRules;
};