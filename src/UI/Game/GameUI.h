#pragma once

#include <memory>

#include "Input/MouseInput.h"

namespace UI
{
    class Material;
    class Text;
}; // namespace UI

class GameUIManager
{
public:
    static void Init();
    static void Update();
    static void Shutdown();
};

class GameUI : MouseInput::EventListener
{
    friend class GameUIManager;

public:
    virtual ~GameUI() = default;

protected:
    virtual void Init()     = 0;
    virtual void Update()   = 0;
    virtual void Shutdown() = 0;

    bool IsFinished() const { return mIsFinished; }
    void SetAsFinished() { mIsFinished = true; }

    bool mIsFinished = false;
};
