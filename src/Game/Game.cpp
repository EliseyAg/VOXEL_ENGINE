#include "Game.hpp"
#include "Player.hpp"
#include "../Rendering/OpenGL/ShaderProgram.hpp"
#include "../Rendering/OpenGL/Renderer.hpp"
#include "../Rendering/OpenGL/Mesh.hpp"
#include "../Rendering/OpenGL/Camera.hpp"
#include "../Rendering/Voxels/Chunk.hpp"
#include "../Rendering/Voxels/Chunks.hpp"
#include "../Rendering/Voxels/Voxel.hpp"
#include "../Rendering/VoxelRenderer.hpp"
#include "../Rendering/LineBatch.hpp"
#include "../Resources/ResourceManager.hpp"
#include "../Events/Input.hpp"
#include "../Events/Keys.hpp"
#include "../Lighting/LightSolver.hpp"
#include "../Lighting/LightMap.hpp"

#include <iostream>

namespace Game
{
    bool perspective_camera = true;

    Rendering::Chunks* chunks;
    Rendering::Mesh** meshes;
    Rendering::VoxelRenderer renderer(1024 * 1024 * 8);
    Rendering::LineBatch* lineBatch = new Rendering::LineBatch(4096);

    Lighting::LightSolver* solverR;
    Lighting::LightSolver* solverG;
    Lighting::LightSolver* solverB;
    Lighting::LightSolver* solverS;

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
                    int x = (int)iend.x;
                    int y = (int)iend.y;
                    int z = (int)iend.z;
                    chunks->set(x, y, z, 0);

                    solverR->remove(x, y, z);
                    solverG->remove(x, y, z);
                    solverB->remove(x, y, z);

                    solverR->solve();
                    solverG->solve();
                    solverB->solve();

                    if (chunks->getLight(x, y + 1, z, 3) == 0xF) {
                        for (int i = y; i >= 0; i--) {
                            if (chunks->get(x, i, z)->id != 0)
                                break;
                            solverS->add(x, i, z, 0xF);
                        }
                    }

                    solverR->add(x, y + 1, z); solverG->add(x, y + 1, z); solverB->add(x, y + 1, z); solverS->add(x, y + 1, z);
                    solverR->add(x, y - 1, z); solverG->add(x, y - 1, z); solverB->add(x, y - 1, z); solverS->add(x, y - 1, z);
                    solverR->add(x + 1, y, z); solverG->add(x + 1, y, z); solverB->add(x + 1, y, z); solverS->add(x + 1, y, z);
                    solverR->add(x - 1, y, z); solverG->add(x - 1, y, z); solverB->add(x - 1, y, z); solverS->add(x - 1, y, z);
                    solverR->add(x, y, z + 1); solverG->add(x, y, z + 1); solverB->add(x, y, z + 1); solverS->add(x, y, z + 1);
                    solverR->add(x, y, z - 1); solverG->add(x, y, z - 1); solverB->add(x, y, z - 1); solverS->add(x, y, z - 1);

                    solverR->solve();
                    solverG->solve();
                    solverB->solve();
                    solverS->solve();
                }
                else if (Events::Input::IsMouseButtonJustPressed(Events::MouseButton::MOUSE_BUTTON_LEFT)) {
                    int x = (int)(iend.x) + (int)(norm.x);
                    int y = (int)(iend.y) + (int)(norm.y);
                    int z = (int)(iend.z) + (int)(norm.z);
                    chunks->set(x, y, z, player.get_current());
                    solverR->remove(x, y, z);
                    solverG->remove(x, y, z);
                    solverB->remove(x, y, z);
                    solverS->remove(x, y, z);
                    for (int i = y - 1; i >= 0; i--) {
                        solverS->remove(x, i, z);
                        if (i == 0 || chunks->get(x, i - 1, z)->id != 0) {
                            break;
                        }
                    }
                    solverR->solve();
                    solverG->solve();
                    solverB->solve();
                    solverS->solve();
                    if (player.get_current() == 6) {
                        solverR->add(x, y, z, 1);
                        solverG->add(x, y, z, 1);
                        solverB->add(x, y, z, 1);
                        solverR->solve();
                        solverG->solve();
                        solverB->solve();
                    }
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


        solverR = new Lighting::LightSolver(chunks, 0);
        solverG = new Lighting::LightSolver(chunks, 1);
        solverB = new Lighting::LightSolver(chunks, 2);
        solverS = new Lighting::LightSolver(chunks, 3);

        for (int y = 0; y < chunks->m_h * CHUNK_D; y++) {
            for (int x = 0; x < chunks->m_w * CHUNK_W; x++) {
                for (int z = 0; z < chunks->m_d * CHUNK_H; z++) {
                    Rendering::Voxel* vox = chunks->get(x, y, z);
                    if (vox->id == 0) {
                        solverR->add(x, y, z, 15);
                        solverG->add(x, y, z, 15);
                        solverB->add(x, y, z, 15);
                    }
                }
            }
        }

        for (int z = 0; z < chunks->m_d * CHUNK_D; z++) {
            for (int x = 0; x < chunks->m_w * CHUNK_W; x++) {
                for (int y = chunks->m_h * CHUNK_H - 1; y >= 0; y--) {
                    Rendering::Voxel* vox = chunks->get(x, y, z);
                    if (vox->id != 6) {
                        break;
                    }
                    chunks->getChunkByVoxel(x, y, z)->lightMap->setS(x % CHUNK_W, y % CHUNK_H, z % CHUNK_D, 0xF);
                }
            }
        }

        for (int z = 0; z < chunks->m_d * CHUNK_D; z++) {
            for (int x = 0; x < chunks->m_w * CHUNK_W; x++) {
                for (int y = chunks->m_h * CHUNK_H - 1; y >= 0; y--) {
                    Rendering::Voxel* vox = chunks->get(x, y, z);
                    if (vox->id != 0) {
                        break;
                    }
                    if (
                        chunks->getLight(x - 1, y, z, 3) == 0 ||
                        chunks->getLight(x + 1, y, z, 3) == 0 ||
                        chunks->getLight(x, y - 1, z, 3) == 0 ||
                        chunks->getLight(x, y + 1, z, 3) == 0 ||
                        chunks->getLight(x, y, z - 1, 3) == 0 ||
                        chunks->getLight(x, y, z + 1, 3) == 0
                        ) {
                        solverS->add(x, y, z);
                    }
                    chunks->getChunkByVoxel(x, y, z)->lightMap->setS(x % CHUNK_W, y % CHUNK_H, z % CHUNK_D, 0xF);
                }
            }
        }

        solverR->solve();
        solverG->solve();
        solverB->solve();
        solverS->solve();

        player.set_viewport_size(static_cast<float>(m_windowSize.x), static_cast<float>(m_windowSize.y));
        pDefaultShaderProgram = Resources::ResourceManager::getShaderProgram("DefaultShaderProgram");
        pLinesShaderProgram = Resources::ResourceManager::getShaderProgram("LinesShaderProgram");
        return 0;
    }
}
