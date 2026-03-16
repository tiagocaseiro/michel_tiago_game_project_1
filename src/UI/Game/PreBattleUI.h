#pragma once

#include <memory>

#include "GameUI.h"
#include "Input/MouseInput.h"

namespace UI
{
    class Material;
    class Text;
}; // namespace UI

class PreBattleUI : public GameUI
{
    friend class GameUIManager;

private:
    PreBattleUI() = default;

    void Init() override;
    void Update() override;
    void Shutdown() override;

    void OnMouseLeftButtonUp(const float mouseX, const float mouseY) override;
    void OnMouseLeftButtonDown(const float mouseX, const float mouseY) override;

    std::weak_ptr<UI::Material> mStartButtonPanel;
    std::weak_ptr<UI::Text> mStartButtonLabel;
};
