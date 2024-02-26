#include "Rendering/OpenGL/Window.hpp"
#include "Rendering/OpenGL/ShaderProgram.hpp"
#include "Rendering/OpenGL/Texture2D.hpp"
#include "Rendering/OpenGL/VertexBuffer.hpp"
#include "Rendering/OpenGL/IndexBuffer.hpp"
#include "Rendering/OpenGL/VertexArray.hpp"
#include "Rendering/OpenGL/Renderer.hpp"
#include "Rendering/OpenGL/Camera.hpp"
#include "Resources/ResourceManager.hpp"
#include "Events/Event.hpp"
#include "Events/Input.hpp"
#include "Events/Keys.hpp"

#include <memory>
#include <iostream>
#include <glm/vec2.hpp>


GLfloat vertexCoords[] = {
  -0.5f, -0.5f,  0.0f,
  -0.5f,  0.5f,  0.0f,
   0.5f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f
};

const GLuint indices[] = {
            0, 1, 2, 2, 3, 0
};

glm::ivec2 g_windowSize(720, 720);

int main(int argc, char** argv)
{
    float scale[3] = { 1.f, 1.f, 1.f };
    float rotate = 0.f;
    float translate[3] = { 0.f, 0.f, 0.f };

    float camera_position[3] = { 0.f, 0.f, 1.f };
    float camera_rotation[3] = { 0.f, 0.f, 0.f };
    bool perspective_camera = true;

    Rendering::Camera camera;

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
                    //std::cout << "KeyPressed, Repeated" << std::endl;
                }
                else
                {
                    //std::cout << "KeyPressed" << std::endl;
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

    auto pDefaultShaderProgram = Resources::ResourceManager::loadShaderProgram("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
        return -1;
    }

    std::vector<std::string> subTexturesNames = { "GrassTop", "Grass", "Dirt", "Coblestone", "WoodTop", "Wood", "Pusto", "Pusto", "Lystya", "No" };
    auto pTexture = Resources::ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/Blocks.png", std::move(subTexturesNames), 64, 64);

    auto subTexture = pTexture->getSubTexture(std::move("GrassTop"));
    const GLfloat textureCoords[] = {
        // U  V
        subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
        subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
        subTexture.rightTopUV.x, subTexture.rightTopUV.y,
        subTexture.rightTopUV.x, subTexture.leftBottomUV.y
    };

    Rendering::VertexBuffer m_vertexCoordsBuffer;
    Rendering::VertexBuffer m_textureCoordsBuffer;
    Rendering::IndexBuffer m_indexBuffer;
    Rendering::VertexArray m_vertexArray;

    m_vertexCoordsBuffer.init(vertexCoords, 3 * 4 * sizeof(GLfloat));
    Rendering::VertexBufferLayout vertexCoordsLayout;
    vertexCoordsLayout.addElementLayoutFloat(3, false);
    m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

    m_textureCoordsBuffer.init(textureCoords, 2 * 4 * sizeof(GLfloat));
    Rendering::VertexBufferLayout textureCoordsLayout;
    textureCoordsLayout.addElementLayoutFloat(2, false);
    m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

    m_indexBuffer.init(indices, 6 * sizeof(GLuint));

    m_vertexArray.unbind();
    m_indexBuffer.unbind();

    pDefaultShaderProgram->bind();

    while (!m_bCloseWindow)
    {
       Rendering::Renderer::setClearColor(1.0f, 1.0f, 0.0f, 1.0f);
       Rendering::Renderer::clear();

       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_W))
       {
           camera_position[2] += -0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_A))
       {
           camera_position[0] += -0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_S))
       {
           camera_position[2] +=  0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_D))
       {
           camera_position[0] +=  0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_Q))
       {
           camera_position[1] +=  0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_E))
       {
           camera_position[1] += -0.001f;
       }

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

       camera.set_position_rotation(glm::vec3(camera_position[0], camera_position[1], camera_position[2]),
           glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
       camera.set_projection_mode(perspective_camera ? Rendering::Camera::ProjectionMode::Perspective : Rendering::Camera::ProjectionMode::Orthograthic);
       pDefaultShaderProgram->setMatrix4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());

       pTexture->bind();

       Rendering::Renderer::draw(m_vertexArray, m_indexBuffer, *pDefaultShaderProgram);

       m_vertexArray.unbind();

       m_pWindow->on_update();

    }

    m_pWindow = nullptr;

    return 0;
}
