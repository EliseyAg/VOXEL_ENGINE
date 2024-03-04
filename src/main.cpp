#include "Rendering/OpenGL/Window.hpp"
#include "Rendering/OpenGL/ShaderProgram.hpp"
#include "Rendering/OpenGL/Renderer.hpp"
#include "Rendering/OpenGL/Mesh.hpp"
#include "Rendering/OpenGL/Camera.hpp"
#include "Resources/ResourceManager.hpp"
#include "Events/Event.hpp"
#include "Events/Input.hpp"
#include "Events/Keys.hpp"

#include <memory>
#include <iostream>
#include <glm/vec2.hpp>


float vertexCoords[] = {
    //    position            index

    // FRONT
    -1.0f, -1.f, -1.f,        // 0
    -1.0f,  1.f, -1.f,        // 1
    -1.0f,  1.f,  1.f,        // 2
    -1.0f, -1.f,  1.f,        // 3

    // BACK           
     1.0f, -1.f, -1.f,        // 4
     1.0f,  1.f, -1.f,        // 5
     1.0f,  1.f,  1.f,        // 6
     1.0f, -1.f,  1.f,        // 7

    // RIGHT
    -1.0f,  1.f, -1.f,        // 8
     1.0f,  1.f, -1.f,        // 9
     1.0f,  1.f,  1.f,        // 10
    -1.0f,  1.f,  1.f,        // 11

    // LEFT
    -1.0f, -1.f, -1.f,        // 12
     1.0f, -1.f, -1.f,        // 13
     1.0f, -1.f,  1.f,        // 14
    -1.0f, -1.f,  1.f,        // 15

    // TOP
    -1.0f, -1.f,  1.f,        // 16
    -1.0f,  1.f,  1.f,        // 17
     1.0f,  1.f,  1.f,        // 18
     1.0f, -1.f,  1.f,        // 19

    // BOTTOM
    -1.0f, -1.f, -1.f,        // 20
    -1.0f,  1.f, -1.f,        // 21
     1.0f,  1.f, -1.f,        // 22
     1.0f, -1.f, -1.f,        // 23
};

glm::ivec2 g_windowSize(720, 720);

bool isLockCursor = true;

int main(int argc, char** argv)
{
    float scale[3] = { 1.f, 1.f, 1.f };
    float translate[3] = { 0.f, 0.f, 0.f };

    bool perspective_camera = true;

    Rendering::Camera camera{ glm::vec3(-5.f, 0.f, 0.f) };

    Resources::ResourceManager::init(argv[0]);

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
            camera.set_viewport_size(event.width, event.height);
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

    auto pDefaultShaderProgram = Resources::ResourceManager::loadShaderProgram("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
        return -1;
    }

    camera.set_viewport_size(static_cast<float>(g_windowSize.x), static_cast<float>(g_windowSize.y));

    std::vector<std::string> subTexturesNames = { "GrassTop", "GrassLeft", "Dirt", "Coblestone", "WoodTop", "WoodLeft", "Pusto", "Pusto", "Green", "Unknown", "Birch", "Pusto", "Wood_Planks"};
    auto pTexture = Resources::ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/Blocks.png", std::move(subTexturesNames), 16, 16);

    std::vector<std::string> grassTextures = { "GrassTop", "GrassLeft", "Dirt" };
    Rendering::Mesh m_mesh(24, vertexCoords, "DefaultShader", grassTextures);

    Rendering::Renderer::setDepth(true);

    while (!m_bCloseWindow)
    {
       Rendering::Renderer::setClearColor(0.0f, 0.5f, 1.0f, 1.0f);
       Rendering::Renderer::clear();

       bool move_camera = false;
       glm::vec3 movement_delta{ 0, 0, 0 };
       glm::vec3 rotation_delta{ 0, 0, 0 };

       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_W))
       {
           movement_delta.x += 0.0005f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_A))
       {
           movement_delta.y -= 0.0005f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_S))
       {
           movement_delta.x -= 0.0005f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_D))
       {
           movement_delta.y += 0.0005f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_Q))
       {
           movement_delta.z -= 0.0005f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_E))
       {
           movement_delta.z += 0.0005f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_ESCAPE))
       {
           isLockCursor = !isLockCursor;
       }
       if (isLockCursor)
       {
           m_pWindow->LockCursor();
           glm::vec2 current_cursor_position = m_pWindow->get_current_cursor_position();
           rotation_delta.z += glm::degrees(static_cast<float>(g_windowSize.x / 2 - current_cursor_position.x) * 0.005f);
           rotation_delta.y -= glm::degrees(static_cast<float>(g_windowSize.y / 2 - current_cursor_position.y) * 0.005f);
       }

       camera.add_movement_and_rotation(movement_delta, rotation_delta);

       glm::mat4 scale_matrix(scale[0], 0, 0, 0,
           0, scale[1], 0, 0,
           0, 0, scale[2], 0,
           0, 0, 0, 1);
       glm::mat4 rotate_matrix(1, 0, 0, 0,
           0, 1, 0, 0,
           0, 0, 1, 0,
           0, 0, 0, 1);
       glm::mat4 translate_matrix(1, 0, 0, 0,
           0, 1, 0, 0,
           0, 0, 1, 0,
           translate[0], translate[1], translate[2], 1);
       glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
       pDefaultShaderProgram->setMatrix4("model_matrix", model_matrix);

       camera.set_projection_mode(perspective_camera ? Rendering::Camera::ProjectionMode::Perspective : Rendering::Camera::ProjectionMode::Orthograthic);
       pDefaultShaderProgram->setMatrix4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());

       m_mesh.render();

       m_pWindow->on_update();

    }

    m_pWindow = nullptr;

    return 0;
}
