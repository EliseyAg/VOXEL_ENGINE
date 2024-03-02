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
  0.0f, -0.5f, -0.5f,
  0.0f,  0.5f, -0.5f,
  0.0f, -0.5f,  0.5f,
  0.0f,  0.5f,  0.5f,
};

glm::ivec2 g_windowSize(720, 720);

double m_initial_mouse_pos_x = 0.0;
double m_initial_mouse_pos_y = 0.0;

void on_mouse_button_event(const Events::MouseButton button_code,
    const double x_pos,
    const double y_pos,
    const bool pressed)
{
    m_initial_mouse_pos_x = x_pos;
    m_initial_mouse_pos_y = y_pos;
}

int main(int argc, char** argv)
{
    float scale[3] = { 1.f, 1.f, 1.f };
    float rotate = 0.f;
    float translate[3] = { 0.f, 0.f, 0.f };

    bool perspective_camera = true;

    Rendering::Camera camera{ glm::vec3(-5.f, 0.f, 0.f) };

    Resources::ResourceManager::init(argv[0]);

    std::unique_ptr<Rendering::Window> m_pWindow = std::make_unique<Rendering::Window>("VOXEL_ENGINE", g_windowSize.x, g_windowSize.y);

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
            g_windowSize.x = event.width;
            g_windowSize.y = event.height;
            Rendering::Renderer::setViewport(0, 0, event.width, event.height);
        }
    );

    m_pWindow->set_event_callback(
        [&](Events::BaseEvent& event)
        {
            m_event_dispatcher.dispatch(event);
        }
    );

    m_event_dispatcher.add_event_listener<Events::EventKeyPressed>(
        [&](Events::EventKeyPressed& event)
        {
            if (event.key_code <= Events::KeyCode::KEY_Z)
            {
                if (event.repeated)
                {
                    std::cout << "KeyPressed, Repeated" << std::endl;
                }
                else
                {
                    std::cout << "KeyPressed" << std::endl;
                }
            }
            Events::Input::PressKey(event.key_code);
        });

    m_event_dispatcher.add_event_listener<Events::EventKeyReleased>(
        [&](Events::EventKeyReleased& event)
        {
            if (event.key_code <= Events::KeyCode::KEY_Z)
            {
                
            }
            Events::Input::ReleaseKey(event.key_code);
        });

    m_event_dispatcher.add_event_listener<Events::EventMouseButtonPressed>(
        [&](Events::EventMouseButtonPressed& event)
        {
            std::cout << "Mouse" << std::endl;
            Events::Input::PressMouseButton(event.mouse_button);
            on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, true);
        });

    m_event_dispatcher.add_event_listener<Events::EventMouseButtonReleased>(
        [&](Events::EventMouseButtonReleased& event)
        {
            Events::Input::ReleaseMouseButton(event.mouse_button);
            on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, false);
        });

    auto pDefaultShaderProgram = Resources::ResourceManager::loadShaderProgram("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
        return -1;
    }

    std::vector<std::string> subTexturesNames = { "GrassTop", "Grass", "Dirt", "Coblestone", "WoodTop", "Wood", "Pusto", "Pusto", "Lystya", "No" };
    auto pTexture = Resources::ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/Blocks.png", std::move(subTexturesNames), 64, 64);

    Rendering::Mesh m_mesh(4, vertexCoords, "DefaultShader", "No");

    while (!m_bCloseWindow)
    {
       Rendering::Renderer::setClearColor(1.0f, 1.0f, 0.0f, 1.0f);
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
           movement_delta.z += 0.0005f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_E))
       {
           movement_delta.z -= 0.0005f;
       }
       if (Events::Input::IsMouseButtonPressed(Events::MouseButton::MOUSE_BUTTON_RIGHT))
       {
           glm::vec2 current_cursor_position = m_pWindow->get_current_cursor_position();
           if (Events::Input::IsMouseButtonPressed(Events::MouseButton::MOUSE_BUTTON_LEFT))
           {
               camera.move_right(static_cast<float>(current_cursor_position.x - m_initial_mouse_pos_x) / 100.f);
               camera.move_up(static_cast<float>(m_initial_mouse_pos_y - current_cursor_position.y) / 100.f);
           }
           else
           {
               rotation_delta.z += static_cast<float>(m_initial_mouse_pos_x - current_cursor_position.x) / 5.f;
               rotation_delta.y -= static_cast<float>(m_initial_mouse_pos_y - current_cursor_position.y) / 5.f;
           }
           m_initial_mouse_pos_x = current_cursor_position.x;
           m_initial_mouse_pos_y = current_cursor_position.y;
       }

       camera.add_movement_and_rotation(movement_delta, rotation_delta);

       glm::mat4 scale_matrix(scale[0], 0, 0, 0,
           0, scale[1], 0, 0,
           0, 0, scale[2], 0,
           0, 0, 0, 1);
       float rotate_in_radians = glm::radians(rotate);
       glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
           -sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
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
