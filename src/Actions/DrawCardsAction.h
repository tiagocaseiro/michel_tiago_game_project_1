#pragma once

#include "ActionFactory.h"
#include "IAction.h"
#include "Model/TurnManager.h"

#include "Tools/TGUID.h"

#include <nlohmann/json.hpp>

class Board;
struct CardZone;
struct Player;

struct DrawCardsActionParams : public IActionParameters
{
public:
    DrawCardsActionParams(CardZone& cardZone, Player& player, int numToDraw, bool advancesTurn = true)
        : IActionParameters(advancesTurn), mCardZone(cardZone), mPlayer(player), mNumToDraw(numToDraw)
    {

    }

    CardZone& mCardZone;
    Player& mPlayer;
    int mNumToDraw;
    bool mEndsTurn;
};

class DrawCardsAction : public AutoRegisterAction<DrawCardsAction>
{
public:
    DrawCardsAction(Board& board, std::unique_ptr<DrawCardsActionParams> params);

    TurnPhase FollowingTurnPhase() const override { return TurnPhase::ReplenishHand; }
    std::string GetBlockers() const override;
    void Apply() override;
    void Undo() override;
    nlohmann::json ToJson() const override;
    static std::unique_ptr<IAction> FromJson(Board&, const nlohmann::json&);

private:
    bool DrawOneCard(const DrawCardsActionParams& params);
    bool UndrawOneCard(const DrawCardsActionParams& params);
    bool MoveOneCard(CardZone& from, CardZone& to);
};
