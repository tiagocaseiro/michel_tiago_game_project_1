#pragma once

#include <memory>

#include "Input/MouseInput.h"
#include "UI/Game/GameUI.h"

namespace UI
{
    class Material;
    class Text;
}; // namespace UI

class PostBattleUI : public GameUI
{
    friend class GameUIManager;

private:
    PostBattleUI() = default;

    void Init() override;
    void Update() override;
    void Shutdown() override;
};
