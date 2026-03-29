#pragma once

#include "Model/Player.h"
#include "Tools/TGUID.h"

#include <unordered_map>

class PlayerManager
{
    static PlayerManager& Instance();

    Player& GetPlayer(TGUID<Player> playerID);

private:
    PlayerManager();
    std::unordered_map<TGUID<Player>, Player> mAllPlayers;
};
