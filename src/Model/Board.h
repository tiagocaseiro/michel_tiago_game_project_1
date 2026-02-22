#pragma once

#include <vector>

#include "Flag.h"
#include "Rules/CardZoneRules.h"
#include "Rules/InitialSetup.h"

struct Player;

struct Board
{
    Board(Player& p1, Player& p2, const InitialSetup& initialSetup = {});

    Player& mPlayer1;
    Player& mPlayer2;
    std::vector<Flag> mFlags;

    CardZone mTroopDeck{CardZoneRules::TroopDeck()};
    CardZone mTacticsDeck{CardZoneRules::TacticsDeck()};
    CardZone mDiscardPile{CardZoneRules::DiscardPile()};

    private:
    void ConstructTroopsDeck(const InitialSetup& setup);
    void ConstructTacticsDeck(const InitialSetup& setup);
};