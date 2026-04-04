#pragma once

#include <memory>

#include "GameUI.h"
#include "Input/MouseInput.h"

namespace UI
{
    class StackPanel;
}; // namespace UI

class Board;

class BattleUI : public GameUI
{
    friend class GameUIManager;

private:
    BattleUI() = default;

    void Init() override;
    void Update(const GameManager& model) override;
    void Shutdown() override;

    void InitializeFlags(const Board& board);

    std::shared_ptr<UI::StackPanel> mFlagsPanel;
    std::shared_ptr<UI::StackPanel> mPlayer1HandPanel;
    std::shared_ptr<UI::StackPanel> mPlayer2HandPanel;
};
