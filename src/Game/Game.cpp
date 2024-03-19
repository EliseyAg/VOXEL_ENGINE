#include "Game.hpp"
#include "../Rendering/OpenGL/ShaderProgram.hpp"
#include "../Rendering/OpenGL/Renderer.hpp"
#include "../Rendering/OpenGL/Mesh.hpp"
#include "../Rendering/OpenGL/Camera.hpp"
#include "../Rendering/Voxels/Chunk.hpp"
#include "../Rendering/Voxels/Chunks.hpp"
#include "../Rendering/VoxelRenderer.hpp"
#include "../Resources/ResourceManager.hpp"
#include "../Events/Input.hpp"
#include "../Events/Keys.hpp"

namespace Game
{
    float scale[3] = { 1.f, 1.f, 1.f };
    float translate[3] = { 0.f, 0.f, 0.f };

    bool perspective_camera = true;

    Rendering::Chunks* chunks;
    Rendering::Mesh** meshes;
    Rendering::VoxelRenderer renderer(1024 * 1024 * 8);

    Rendering::Camera camera{ glm::vec3(16.f) };
    std::shared_ptr<Rendering::ShaderProgram> pDefaultShaderProgram;

    Game::Game(const glm::ivec2& windowSize)
        : m_eCurrentGameState(EGameState::Active)
        , m_windowSize(windowSize)
    {

    }

    Game::~Game()
    {

    }

    void Game::update_winsize(glm::ivec2& windowSize)
    {
        m_windowSize = windowSize;
        camera.set_viewport_size(m_windowSize.x, m_windowSize.y);
    }

    void Game::render(glm::vec2& current_cursor_position)
    {
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
        if (Events::Input::IsKeyJustPressed(Events::KeyCode::KEY_ESCAPE))
        {
            switch (m_eCurrentGameState)
            {
            case EGameState::Active:
                m_eCurrentGameState = EGameState::Pause;
                break;
            case EGameState::Pause:
                m_eCurrentGameState = EGameState::Active;
                break;
            }
        }
        switch (m_eCurrentGameState)
        {
        case EGameState::Active:
            rotation_delta.z += glm::degrees(static_cast<float>(m_windowSize.x / 2 - current_cursor_position.x) * 0.01f);
            rotation_delta.y -= glm::degrees(static_cast<float>(m_windowSize.y / 2 - current_cursor_position.y) * 0.01f);
            glm::vec3 end;
            glm::vec3 norm;
            glm::vec3 iend;
            Rendering::Voxel* vox = chunks->rayCast(camera.get_position(), camera.get_direction(), 10.0f, end, norm, iend);
            if (vox != nullptr) {
                if (Events::Input::IsMouseButtonJustPressed(Events::MouseButton::MOUSE_BUTTON_RIGHT)) {
                    chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
                }
                else if (Events::Input::IsMouseButtonJustPressed(Events::MouseButton::MOUSE_BUTTON_LEFT)) {
                    chunks->set((int)(iend.x) + (int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), 5);
                }
            }
            break;
        //case EGameState::Pause:
        //    break;
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
    }

    void Game::update(const uint64_t delta)
    {
        camera.set_viewport_size(m_windowSize.x, m_windowSize.y);
    }

    bool Game::init()
    {
        chunks = new Rendering::Chunks(8, 8, 1);
        meshes = new Rendering::Mesh * [chunks->volume];
        for (size_t i = 0; i < chunks->volume; i++)
            meshes[i] = nullptr;

        camera.set_viewport_size(static_cast<float>(m_windowSize.x), static_cast<float>(m_windowSize.y));
        pDefaultShaderProgram = Resources::ResourceManager::getShaderProgram("DefaultShader");
        return 0;
    }
}
