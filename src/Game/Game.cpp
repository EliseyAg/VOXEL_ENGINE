#include "Game.hpp"
#include "Player.hpp"
#include "../Rendering/OpenGL/ShaderProgram.hpp"
#include "../Rendering/OpenGL/Renderer.hpp"
#include "../Rendering/OpenGL/Mesh.hpp"
#include "../Rendering/OpenGL/Camera.hpp"
#include "../Rendering/Voxels/Chunk.hpp"
#include "../Rendering/Voxels/Chunks.hpp"
#include "../Rendering/VoxelRenderer.hpp"
#include "../Rendering/LineBatch.hpp"
#include "../Resources/ResourceManager.hpp"
#include "../Events/Input.hpp"
#include "../Events/Keys.hpp"

#include <iostream>

namespace Game
{
    bool perspective_camera = true;

    Rendering::Chunks* chunks;
    Rendering::Mesh** meshes;
    Rendering::VoxelRenderer renderer(1024 * 1024 * 8);
    Rendering::LineBatch* lineBatch = new Rendering::LineBatch(4096);

    Player player{ glm::vec3(16.f) };
    std::shared_ptr<Rendering::ShaderProgram> pDefaultShaderProgram;
    std::shared_ptr<Rendering::ShaderProgram> pLinesShaderProgram;

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
        player.set_viewport_size(m_windowSize.x, m_windowSize.y);
    }

    void Game::render()
    {
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

        player.set_projection_mode(perspective_camera ? Rendering::Camera::ProjectionMode::Perspective : Rendering::Camera::ProjectionMode::Orthograthic);
        pDefaultShaderProgram->bind();
        pDefaultShaderProgram->setMatrix4("projection_view_matrix", player.get_projection_matrix() * player.get_view_matrix());

        glm::mat4 model_matrix(1.0f);
        for (size_t i = 0; i < chunks->volume; i++) {
            Rendering::Chunk* chunk = chunks->chunks[i];
            Rendering::Mesh* mesh = meshes[i];
            model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(chunk->m_x * CHUNK_W + 0.5f, chunk->m_y * CHUNK_H + 0.5f, chunk->m_z * CHUNK_D + 0.5f));
            pDefaultShaderProgram->setMatrix4("model_matrix", model_matrix);
            mesh->render();
        }

        pLinesShaderProgram->bind();
        pLinesShaderProgram->setMatrix4("projview", player.get_projection_matrix() * player.get_view_matrix());
        lineBatch->render();
    }

    void Game::update(const uint64_t delta, glm::vec2& current_cursor_position)
    {
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
        if (Events::Input::IsKeyJustPressed(Events::KeyCode::KEY_F3))
        {
            unsigned char* buffer = new unsigned char[chunks->volume * CHUNK_VOL];
            chunks->write(buffer);
            Resources::ResourceManager::writeBinaryFile("res/worlds/world.bin", (const char*)buffer, chunks->volume * CHUNK_VOL);

            delete[] buffer;
        }
        if (Events::Input::IsKeyJustPressed(Events::KeyCode::KEY_F4))
        {
            unsigned char* buffer = new unsigned char[chunks->volume * CHUNK_VOL];
            Resources::ResourceManager::readBinaryFile("res/worlds/world.bin", (char*)buffer, chunks->volume * CHUNK_VOL);
            chunks->read(buffer);

            delete[] buffer;
        }

        switch (m_eCurrentGameState)
        {
        case EGameState::Active:
            glm::vec3 rotation_delta{ 0, 0, 0 };
            rotation_delta.z += glm::degrees(static_cast<float>(m_windowSize.x / 2 - current_cursor_position.x)) * delta * 0.00000001f;
            rotation_delta.y -= glm::degrees(static_cast<float>(m_windowSize.y / 2 - current_cursor_position.y)) * delta * 0.00000001f;
            glm::vec3 end;
            glm::vec3 norm;
            glm::vec3 iend;
            Rendering::Voxel* vox = chunks->rayCast(player.get_position(), player.get_direction(), 10.0f, end, norm, iend);
            if (vox != nullptr)
            {
                lineBatch->box(iend.x + 0.5f, iend.y + 0.5f, iend.z + 0.5f, 1.005f, 1.005f, 1.005f, 0, 0, 0, 0.5f);
                if (Events::Input::IsMouseButtonJustPressed(Events::MouseButton::MOUSE_BUTTON_RIGHT)) {
                    chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
                }
                else if (Events::Input::IsMouseButtonJustPressed(Events::MouseButton::MOUSE_BUTTON_LEFT)) {
                    chunks->set((int)(iend.x) + (int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), player.get_current());
                }
            }
            player.add_rotation(rotation_delta);
            break;
            //case EGameState::Pause:
            //    break;
        }
        player.on_update(delta);
    }

    bool Game::init()
    {
        chunks = new Rendering::Chunks(8, 8, 1);
        meshes = new Rendering::Mesh * [chunks->volume];
        for (size_t i = 0; i < chunks->volume; i++)
            meshes[i] = nullptr;

        player.set_viewport_size(static_cast<float>(m_windowSize.x), static_cast<float>(m_windowSize.y));
        pDefaultShaderProgram = Resources::ResourceManager::getShaderProgram("DefaultShaderProgram");
        pLinesShaderProgram = Resources::ResourceManager::getShaderProgram("LinesShaderProgram");
        return 0;
    }
}
