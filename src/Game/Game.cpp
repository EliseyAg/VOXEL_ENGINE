#include "Game.hpp"
#include "Player.hpp"
#include "World.hpp";
#include "../Rendering/OpenGL/ShaderProgram.hpp"
#include "../Rendering/OpenGL/Renderer.hpp"
#include "../Rendering/OpenGL/Mesh.hpp"
#include "../Rendering/OpenGL/Camera.hpp"
#include "../Rendering/LineBatch.hpp"
#include "../Resources/ResourceManager.hpp"
#include "../Events/Input.hpp"
#include "../Events/Keys.hpp"
#include "../Lighting/Lighting.hpp"

#include <iostream>

namespace Game
{
    bool perspective_camera = true;

    World world(8, 8, 1);
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
        player.set_projection_mode(perspective_camera ? Rendering::Camera::ProjectionMode::Perspective : Rendering::Camera::ProjectionMode::Orthograthic);
        pDefaultShaderProgram->bind();
        pDefaultShaderProgram->setMatrix4("projection_view_matrix", player.get_projection_matrix() * player.get_view_matrix());

        world.render(pDefaultShaderProgram);

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
            unsigned char* buffer = new unsigned char[world.getChunksVolume() * CHUNK_VOL];
            //chunks->write(buffer);
            Resources::ResourceManager::writeBinaryFile("res/worlds/world.bin", (const char*)buffer, world.getChunksVolume() * CHUNK_VOL);

            delete[] buffer;
        }
        if (Events::Input::IsKeyJustPressed(Events::KeyCode::KEY_F4))
        {
            unsigned char* buffer = new unsigned char[world.getChunksVolume() * CHUNK_VOL];
            Resources::ResourceManager::readBinaryFile("res/worlds/world.bin", (char*)buffer, world.getChunksVolume() * CHUNK_VOL);
            //chunks->read(buffer);
            Lighting::Lighting::init(world.getChunks());

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
            Rendering::Voxel* vox = world.rayCast(player.get_position(), player.get_direction(), 10.0f, end, norm, iend);
            if (vox != nullptr)
            {
                lineBatch->box(iend.x + 0.5f, iend.y + 0.5f, iend.z + 0.5f, 1.005f, 1.005f, 1.005f, 0, 0, 0, 0.5f);
                if (Events::Input::IsMouseButtonJustPressed(Events::MouseButton::MOUSE_BUTTON_RIGHT)) {
                    int x = (int)iend.x;
                    int y = (int)iend.y;
                    int z = (int)iend.z;
                    world.set(x, y, z, 0);
                    Lighting::Lighting::del(world.getChunks(), x, y, z);
                }
                else if (Events::Input::IsMouseButtonJustPressed(Events::MouseButton::MOUSE_BUTTON_LEFT)) {
                    int x = (int)(iend.x) + (int)(norm.x);
                    int y = (int)(iend.y) + (int)(norm.y);
                    int z = (int)(iend.z) + (int)(norm.z);
                    world.set(x, y, z, player.get_current());
                    Lighting::Lighting::add(world.getChunks(), x, y, z, player.get_current() == 5);
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
        Lighting::Lighting::init(world.getChunks());

        player.set_viewport_size(static_cast<float>(m_windowSize.x), static_cast<float>(m_windowSize.y));
        pDefaultShaderProgram = Resources::ResourceManager::getShaderProgram("DefaultShaderProgram");
        pLinesShaderProgram = Resources::ResourceManager::getShaderProgram("LinesShaderProgram");
        return 0;
    }
}
