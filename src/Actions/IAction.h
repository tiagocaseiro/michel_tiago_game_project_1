#pragma once

#include <memory>

#include <nlohmann/json.hpp>

class Board;
enum class TurnPhase;

struct IActionParameters
{
    IActionParameters(bool advancesTurn) : mAdvancesTurn(advancesTurn) {}
    bool mAdvancesTurn;
};

class IAction
{
public:
    IAction(Board& board, std::unique_ptr<IActionParameters> params) : mBoard(board), mParams(std::move(params)) {}

    IAction(const IAction&)            = delete;
    IAction& operator=(const IAction&) = delete;

    IAction(IAction&&) noexcept            = default;
    IAction& operator=(IAction&&) noexcept = delete;

    virtual ~IAction() = default;

    virtual TurnPhase FollowingTurnPhase() const = 0;
    virtual std::string GetTypeName()       = 0;
    virtual std::string GetBlockers() const = 0;
    virtual void Apply()                    = 0;
    virtual void Undo()                     = 0;
    virtual nlohmann::json ToJson() const   = 0;

    bool CanApply() const { return GetBlockers().empty(); }
    bool AdvancesTurn() const { return mParams ? mParams->mAdvancesTurn : false; }

protected:
    Board& mBoard;
    std::unique_ptr<IActionParameters> mParams;
};

template <typename T>
concept IsAction = std::is_base_of_v<IAction, T>;
