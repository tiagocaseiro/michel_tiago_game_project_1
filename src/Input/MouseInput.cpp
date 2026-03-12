#include "MouseInput.h"

#include <algorithm>
#include <vector>

#include <SDL3/SDL.h>

namespace MouseInput
{
    std::vector<EventListener*> sEventListeners;

    static float sMouseX = 0;
    static float sMouseY = 0;

    static void OnMouseMotion() { SDL_GetMouseState(&sMouseX, &sMouseX); }

    static void OnMouseButtonDown()
    {
        for(EventListener* const eventListener : sEventListeners)
        {
            if(eventListener)
            {
                eventListener->OnMouseLeftButtonDown(sMouseX, sMouseX);
            }
        }
    }

    static void OnMouseButtonUp()
    {
        for(EventListener* const eventListener : sEventListeners)
        {
            if(eventListener)
            {
                eventListener->OnMouseLeftButtonUp(sMouseX, sMouseX);
            }
        }
    }

    void HandleEvent(const SDL_Event& event)
    {
        switch(event.type)
        {
            case SDL_EVENT_MOUSE_MOTION:
                OnMouseMotion();
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                OnMouseButtonDown();
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                OnMouseButtonUp();
                break;
        }
    }

    EventListener::EventListener() { sEventListeners.push_back(this); }

    EventListener::~EventListener()
    {
        auto it = std::remove(std::begin(sEventListeners), std::end(sEventListeners), this);
        sEventListeners.erase(it);
    }

} // namespace MouseInput