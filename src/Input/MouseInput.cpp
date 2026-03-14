#include "MouseInput.h"

#include <algorithm>
#include <vector>

#include <SDL3/SDL.h>

namespace MouseInput
{
    std::vector<EventListener*> sEventListeners;

    static float sMouseX = 0;
    static float sMouseY = 0;

    void NotifyMouseLeftButtonUp()
    {
        for(EventListener* const eventListener : sEventListeners)
        {
            if(eventListener)
            {
                eventListener->OnMouseLeftButtonUp(sMouseX, sMouseY);
            }
        }
    }

    void NotifyMouseLeftButtonDown()
    {
        for(EventListener* const eventListener : sEventListeners)
        {
            if(eventListener)
            {
                eventListener->OnMouseLeftButtonDown(sMouseX, sMouseY);
            }
        }
    }

    static void OnMouseMotion(const SDL_MouseMotionEvent event)
    {
        sMouseX = event.x;
        sMouseY = event.y;
    }

    static void OnMouseButtonDown(const SDL_MouseButtonEvent& event)
    {
        switch(event.button)
        {
            case SDL_BUTTON_LEFT:
                NotifyMouseLeftButtonDown();
                break;
            default:
                break;
        }
    }

    static void OnMouseButtonUp(const SDL_MouseButtonEvent& event)
    {
        switch(event.button)
        {
            case SDL_BUTTON_LEFT:
                NotifyMouseLeftButtonUp();
                break;
            default:
                break;
        }
    }

    void HandleEvent(const SDL_Event& event)
    {
        switch(event.type)
        {
            case SDL_EVENT_MOUSE_MOTION:
                OnMouseMotion(event.motion);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                OnMouseButtonDown(event.button);
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                OnMouseButtonUp(event.button);
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