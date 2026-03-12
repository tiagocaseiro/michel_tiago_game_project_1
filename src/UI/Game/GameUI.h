#pragma

#include "Input/MouseInput.h"

class GameUI : MouseInput::EventListener
{
private:
    GameUI() = default;

public:
    static GameUI& Instance();

    void InitializeStartUpUI();

    void OnMouseLeftButtonUp(const float mouseX, const float mouseY) override;
};
