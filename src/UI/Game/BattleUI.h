#pragma once

#include <memory>

#include "GameUI.h"
#include "Input/MouseInput.h"

namespace UI
{
    class Material;
    class Text;
}; // namespace UI

class BattleUI : public GameUI
{
    friend class GameUIManager;

private:
    BattleUI() = default;

    void Init() override;
    void Update() override;
    void Shutdown() override;
};
