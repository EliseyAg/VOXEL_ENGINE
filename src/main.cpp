#include "Rendering/OpenGL/Window.hpp"
#include "Rendering/OpenGL/ShaderProgram.hpp"
#include "Rendering/OpenGL/Renderer.hpp"
#include "Rendering/OpenGL/Mesh.hpp"
#include "Rendering/OpenGL/Camera.hpp"
#include "Rendering/Voxels/Chunk.hpp"
#include "Rendering/Voxels/Chunks.hpp"
#include "Rendering/VoxelRenderer.hpp"
#include "Resources/ResourceManager.hpp"
#include "Events/Event.hpp"
#include "Events/Input.hpp"
#include "Events/Keys.hpp"

#include <memory>
#include <iostream>
#include <glm/vec2.hpp>

glm::ivec2 g_windowSize(720, 720);

bool isLockCursor = true;

int main(int argc, char** argv)
{
    float scale[3] = { 1.f, 1.f, 1.f };
    float translate[3] = { 0.f, 0.f, 0.f };

    bool perspective_camera = true;

    Rendering::Camera camera{ glm::vec3(16.f, 16.f, 16.f) };

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

    Rendering::Chunks* chunks = new Rendering::Chunks(4, 4, 4);
    Rendering::Mesh** meshes = new Rendering::Mesh * [chunks->volume];
    for (size_t i = 0; i < chunks->volume; i++)
        meshes[i] = nullptr;
    Rendering::VoxelRenderer renderer(1024 * 1024 * 8);

    camera.set_viewport_size(static_cast<float>(g_windowSize.x), static_cast<float>(g_windowSize.y));

    std::vector<std::string> subTexturesNames = { "Air", "GrassTop", "GrassLeft", "Dirt", "Coblestone", "WoodTop", "WoodLeft", "Pusto", "Green", "Unknown", "Birch", "Pusto", "Wood_Planks"};
    auto pTexture = Resources::ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/Blocks.png", std::move(subTexturesNames), 16, 16);

    std::vector<std::string> grassTextures = { "GrassTop", "GrassLeft", "Dirt" };

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
           movement_delta.x += 0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_A))
       {
           movement_delta.y -= 0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_S))
       {
           movement_delta.x -= 0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_D))
       {
           movement_delta.y += 0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_Q))
       {
           movement_delta.z -= 0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_E))
       {
           movement_delta.z += 0.001f;
       }
       if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_ESCAPE))
       {
           isLockCursor = !isLockCursor;
       }
       if (isLockCursor)
       {
           m_pWindow->LockCursor();
           glm::vec2 current_cursor_position = m_pWindow->get_current_cursor_position();
           rotation_delta.z += glm::degrees(static_cast<float>(g_windowSize.x / 2 - current_cursor_position.x) * 0.01f);
           rotation_delta.y -= glm::degrees(static_cast<float>(g_windowSize.y / 2 - current_cursor_position.y) * 0.01f);
           glm::vec3 end;
           glm::vec3 norm;
           glm::vec3 iend;
           Rendering::Voxel* vox = chunks->rayCast(camera.get_position(), camera.get_direction(), 10.0f, end, norm, iend);
           if (vox != nullptr) {
               if (Events::Input::IsMouseButtonPressed(Events::MouseButton::MOUSE_BUTTON_RIGHT)) {
                   chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
               }
               if (Events::Input::IsMouseButtonPressed(Events::MouseButton::MOUSE_BUTTON_LEFT)) {
                   chunks->set((int)(iend.x) + (int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), 2);
               }
           }
       }
       camera.add_movement_and_rotation(movement_delta, rotation_delta);

       Rendering::Chunk* closes[27];
       for (size_t i = 0; i < chunks->volume; i++) {
           Rendering::Chunk* chunk = chunks->chunks[i];
           if (!chunk->modified)
               continue;
           chunk->modified = false;
           if (meshes[i] != nullptr)
               delete meshes[i];

           for (int i = 0; i < 27; i++)
               closes[i] = nullptr;
           for (size_t j = 0; j < chunks->volume; j++) {
               Rendering::Chunk* other = chunks->chunks[j];

               int ox = other->m_x - chunk->m_x;
               int oy = other->m_y - chunk->m_y;
               int oz = other->m_z - chunk->m_z;

               if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
                   continue;

               ox += 1;
               oy += 1;
               oz += 1;
               closes[(oy * 3 + oz) * 3 + ox] = other;
           }
           Rendering::Mesh* mesh = renderer.render(chunk, (const Rendering::Chunk**)closes);
           meshes[i] = mesh;
       }

       camera.set_projection_mode(perspective_camera ? Rendering::Camera::ProjectionMode::Perspective : Rendering::Camera::ProjectionMode::Orthograthic);
       pDefaultShaderProgram->setMatrix4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());

       glm::mat4 model_matrix(1.0f);
       for (size_t i = 0; i < chunks->volume; i++) {
           Rendering::Chunk* chunk = chunks->chunks[i];
           Rendering::Mesh* mesh = meshes[i];
           model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(chunk->m_x * CHUNK_W + 0.5f, chunk->m_y * CHUNK_H + 0.5f, chunk->m_z * CHUNK_D + 0.5f));
           pDefaultShaderProgram->setMatrix4("model_matrix", model_matrix);
           mesh->render();
       }

       m_pWindow->on_update();

    }

    m_pWindow = nullptr;

    return 0;
}
