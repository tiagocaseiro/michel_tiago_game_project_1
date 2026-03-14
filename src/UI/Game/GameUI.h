#pragma once

#include <memory>

#include "Input/MouseInput.h"

namespace UI
{
    class Material;
    class Text;
}; // namespace UI

class GameUI : MouseInput::EventListener
{
private:
    enum class State
    {
        PreBattle,
        Battle,
        PostBattle,
        Invalid
    };

public:
    static GameUI& Instance();

    void Init();
    void Update();
    void Shutdown();

private:
    GameUI();

    void EnterState(State const state);
    void ExitState(State const state);

    void EnterPreBattleState();
    void EnterBattleState();
    void EnterPostBattleState();

    void UpdatePreBattleState();
    void UpdateBattleState();
    void UpdatePostBattleState();

    void ExitPreBattleState();
    void ExitBattleState();
    void ExitPostBattleState();

    void OnMouseLeftButtonUp(const float mouseX, const float mouseY) override;
    void OnMouseLeftButtonDown(const float mouseX, const float mouseY) override;

    std::weak_ptr<UI::Material> mStartButtonPanel;
    std::weak_ptr<UI::Text> mStartButtonLabel;

    State mState;
};
