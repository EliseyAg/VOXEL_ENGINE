#include <iostream>
#include "Rendering/OpenGL/Window.hpp"
#include "Events/Event.hpp"
#include <memory>

int main()
{
    std::unique_ptr<Rendering::Window> m_pWindow = std::make_unique<Rendering::Window>("Hello World", 640, 480);

    Events::EventDispatcher m_event_dispatcher;
    bool m_bCloseWindow = false;

    m_event_dispatcher.add_event_listener<Events::EventWindowClose>(
        [&](Events::EventWindowClose& event)
        {
            m_bCloseWindow = true;
        }
    );

    m_event_dispatcher.add_event_listener<Events::EventWindowResize>(
        [&](Events::EventWindowResize& event)
        {
        }
    );

    m_pWindow->set_event_callback(
        [&](Events::BaseEvent& event)
        {
            m_event_dispatcher.dispatch(event);
        }
    );

    while (!m_bCloseWindow)
    {
       m_pWindow->on_update();
    }

    m_pWindow = nullptr;

    return 0;
}
