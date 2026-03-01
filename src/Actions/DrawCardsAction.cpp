#include "DrawCardsAction.h"

#include "Model/Board.h"
#include "Model/Player.h"
#include "Model/Card.h"

	DrawCardsAction::DrawCardsAction(Board& board, std::unique_ptr<DrawCardsActionParams> params)
		: AutoRegisterAction(board, std::move(params))
	{
	}

    std::string DrawCardsAction::GetBlockers() const
	{
	    const auto& params = *static_cast<DrawCardsActionParams*>(mParams.get());
	    const auto cardsLeftInDeck =  params.mCardZone.mCards.size();
	    const bool enoughCardsToDraw = cardsLeftInDeck >= params.mNumToDraw;
	    if (!enoughCardsToDraw)
	    {
	        return std::format("Trying to draw {} cards from deck with only {} cards left", params.mNumToDraw, cardsLeftInDeck);
	    }

	    const auto& handSizeLimit = params.mPlayer.mPlayerHand.mRules.mSlotLimit;
	    if (!handSizeLimit)
	    {
	        return {};
	    }

	    const auto cardSlotsLeftInPlayerHand = *handSizeLimit - params.mPlayer.mPlayerHand.mCards.size();
	    if (cardSlotsLeftInPlayerHand < params.mNumToDraw)
	    {
	        return std::format("Player has {} card slots left but trying to draw {} cards", cardSlotsLeftInPlayerHand, params.mNumToDraw);
	    }

        return {};
	}

    void DrawCardsAction::Apply()
	{
	    const auto blockers = GetBlockers();
	    if (!blockers.empty())
	    {
	        Logging::FLogError("Cannot apply DrawCardsAction: {}", blockers);
	        return;
	    }

	    auto& params = *static_cast<DrawCardsActionParams*>(mParams.get());
		for (int i = 0; i < params.mNumToDraw; ++i)
		{
            if (!DrawOneCard(params))
            {
                Logging::LogError("DrawCardsAction failed UNEXPECTEDLY. Investigate");
                return;
            }
		}
	}

	void DrawCardsAction::Undo()
	{
		auto& params = *static_cast<DrawCardsActionParams*>(mParams.get());
		for (int i = 0; i < params.mNumToDraw; ++i)
		{
		    if (!UndrawOneCard(params))
		    {
		        Logging::LogError("Failed to undo DrawCardsAction");
		    }
		}
	}

	nlohmann::json DrawCardsAction::ToJson() const
	{
		const DrawCardsActionParams& params = *(static_cast<DrawCardsActionParams*>(mParams.get()));
		nlohmann::json j;

		j["NumToDraw"] = params.mNumToDraw;
		j["CardZoneID"] = params.mCardZone.mGuid.ID();
		j["PlayerID"] = params.mPlayer.mGuid.ID();

		return j;
	}
    bool DrawCardsAction::DrawOneCard(const DrawCardsActionParams& params)
	{
        return  MoveOneCard(params.mCardZone, params.mPlayer.mPlayerHand);
	}

    bool DrawCardsAction::UndrawOneCard(const DrawCardsActionParams& params)
	{
        return MoveOneCard(params.mPlayer.mPlayerHand, params.mCardZone);
	}

    bool DrawCardsAction::MoveOneCard(CardZone& from, CardZone& to)
	{
	    if (from.mCards.empty())
	    {
	        return false;
	    }

	    const auto cardToDraw = from.mCards.back();

	    if (!to.CanHoldCard(cardToDraw))
	    {
	        return false;
	    }

	    to.mCards.push_back(cardToDraw);
	    from.mCards.pop_back();
	    return true;
	}

    std::unique_ptr<IAction> DrawCardsAction::FromJson(Board& board, const nlohmann::json& j)
	{
		const auto numToDraw =j.at("NumToDraw").get<int>();
		const auto deckID = TGUID<CardZone>(j.at("CardZoneID").get<int>());
		const auto playerID = TGUID<Player>(j.at("PlayerID").get<int>());

		// #TODO_SERIALIZATION: We need to use the IDs to get the deck and player from the Board
	    return nullptr;
	}
