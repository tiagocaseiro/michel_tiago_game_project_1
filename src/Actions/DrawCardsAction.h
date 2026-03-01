#pragma once

#include "ActionFactory.h"
#include "IAction.h"

#include "Tools/TGUID.h"

#include <nlohmann/json.hpp>

class Board;
struct CardZone;
struct Player;

struct DrawCardsActionParams : public IActionParameters
{
public:
    DrawCardsActionParams(CardZone& cardZone, Player& player, int numToDraw)
        : mCardZone(cardZone), mPlayer(player), mNumToDraw(numToDraw)
    {

    }

    CardZone& mCardZone;
    Player& mPlayer;
    int mNumToDraw;
};

class DrawCardsAction : public AutoRegisterAction<DrawCardsAction>
{
public:
    DrawCardsAction(Board& battle, std::unique_ptr<DrawCardsActionParams> params);
    DrawCardsAction(const DrawCardsAction& other) = default;

    std::string GetBlockers() const  override;
    void Apply() override;
    void Undo() override;
    nlohmann::json ToJson() const override;
    static std::unique_ptr<IAction> FromJson(Board&, const nlohmann::json&);

private:
    bool DrawOneCard(const DrawCardsActionParams& params);
    bool UndrawOneCard(const DrawCardsActionParams& params);
    bool MoveOneCard(CardZone& from, CardZone& to);
};
