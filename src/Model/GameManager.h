#pragma once

#include "Board.h"
#include "CardManager.h"
#include "Player.h"
#include "TurnManager.h"

#include "Actions/ActionManager.h"
#include "Rules/FormationRules.h"
#include "Rules/GameplayRules.h"
#include "Rules/InitialSetup.h"

#include <string>

class GameManager
{
public:
    GameManager(const FormationRules& formationRules = {}, const GameplayRules& gameplayRules = {},
                const InitialSetup& initialSetup = {});

    void Save(const std::string& saveName = "autosave") const;

    const Board& GetBoard() const { return mBoard; }

private:
    void StartGame(const InitialSetup& initialSetup);
    void DistributeInitialCards(const InitialSetup& initialSetup, Player& player);

    // #TODO: In order to support save-load, we really need players to live on a player manager
    // and be accessible through their GUIDs
    Player mPlayer1;
    Player mPlayer2;
    Board mBoard;
    ActionManager mActionManager;
    TurnManager mTurnManager;

    // #TODO: We could have the option to data-drive these
    FormationRules mFormationRules;
    GameplayRules mGameplayRules;
};