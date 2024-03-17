#pragma once
#include <glm/vec2.hpp>

namespace Game
{
    class Game
    {
    public:
        enum class EGameState {
            Active,
            Pause
        };

        Game(const glm::ivec2& windowSize);
        ~Game();

        void render(glm::vec2& current_cursor_position);
        void update(const uint64_t delta);
        bool init();

        void update_winsize(glm::ivec2& windowSize);
        EGameState get_GameState() const { return m_eCurrentGameState; };
    private:
        glm::ivec2 m_windowSize;
        EGameState m_eCurrentGameState;
    };
}
