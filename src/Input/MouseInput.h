#pragma once

union SDL_Event;

namespace MouseInput
{
    void HandleEvent(const SDL_Event& event);

    static void OnMouseMotion();

    static void OnMouseButtonDown();

    static void OnMouseButtonUp();

    struct EventListener
    {
        EventListener();
        virtual ~EventListener();
        EventListener(const EventListener&)            = delete;
        EventListener(EventListener&&)                 = delete;
        EventListener& operator=(const EventListener&) = delete;
        EventListener& operator=(EventListener&&)      = delete;

        virtual void OnMouseMotion() {}
        virtual void OnMouseLeftButtonDown(const float mouseX, const float mouseY) {}
        virtual void OnMouseLeftButtonUp(const float mouseX, const float mouseY) {}
    };
} // namespace MouseInput