#include "Rendering/OpenGL/Window.hpp"
#include "Rendering/OpenGL/Renderer.hpp"
#include "Resources/ResourceManager.hpp"
#include "Game/Game.hpp"
#include "Events/Event.hpp"
#include "Events/Input.hpp"
#include "Events/Keys.hpp"

#include <memory>
#include <iostream>
#include <chrono>
#include <glm/vec2.hpp>

glm::ivec2 g_windowSize(720, 720);

int main(int argc, char** argv)
{
    Resources::ResourceManager::setExecutablePath(argv[0]);

    Game::Game m_game(g_windowSize);

    std::unique_ptr<Rendering::Window> m_pWindow = std::make_unique<Rendering::Window>("VOXEL_ENGINE", g_windowSize.x, g_windowSize.y);

    Events::EventDispatcher m_event_dispatcher;
    bool m_bCloseWindow = false;

    m_pWindow->set_event_callback(
        [&](Events::BaseEvent& event)
        {
            m_event_dispatcher.dispatch(event);
        });

    m_event_dispatcher.add_event_listener<Events::EventWindowClose>(
        [&](Events::EventWindowClose& event)
        {
            m_bCloseWindow = true;
        });

    m_event_dispatcher.add_event_listener<Events::EventWindowResize>(
        [&](Events::EventWindowResize& event)
        {
            g_windowSize.x = event.width;
            g_windowSize.y = event.height;
            Rendering::Renderer::setViewport(event.width, event.height, 0, 0);
            m_game.update_winsize(g_windowSize);
        });

    m_event_dispatcher.add_event_listener<Events::EventKeyPressed>(
        [&](Events::EventKeyPressed& event)
        {
            Events::Input::PressKey(event.key_code);
        });

    m_event_dispatcher.add_event_listener<Events::EventKeyReleased>(
        [&](Events::EventKeyReleased& event)
        {
            Events::Input::ReleaseKey(event.key_code);
        });

    m_event_dispatcher.add_event_listener<Events::EventMouseButtonPressed>(
        [&](Events::EventMouseButtonPressed& event)
        {
            Events::Input::PressMouseButton(event.mouse_button);
        });

    m_event_dispatcher.add_event_listener<Events::EventMouseButtonReleased>(
        [&](Events::EventMouseButtonReleased& event)
        {
            Events::Input::ReleaseMouseButton(event.mouse_button);
        });

    Resources::ResourceManager::loadJSONResources("res/resources.json");
    bool resultCode = m_game.init();

    Rendering::Renderer::setDepth(true);

    auto lastTime = std::chrono::high_resolution_clock::now();
    while (!m_bCloseWindow)
    {
       Rendering::Renderer::setClearColor(0.0f, 0.5f, 1.0f, 1.0f);
       Rendering::Renderer::clear();

       auto currentTime = std::chrono::high_resolution_clock::now();
       uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
       lastTime = currentTime;

       m_game.update(duration, m_pWindow->get_current_cursor_position());

       m_game.render();

       switch (m_game.get_GameState())
       {
       case Game::Game::EGameState::Active:
           m_pWindow->LockCursor();
           break;
       case Game::Game::EGameState::Pause:
           break;
       }

       Events::Input::update();
       m_pWindow->on_update();
    }

    Resources::ResourceManager::unloadAllResources();
    m_pWindow = nullptr;

    return resultCode;
}
