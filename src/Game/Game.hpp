#pragma once
#include <glm/vec2.hpp>

namespace Game
{
    class Game
    {
    public:
        Game(const glm::ivec2& windowSize);
        ~Game();

        void render(glm::vec2& current_cursor_position);
        void update(const uint64_t delta);
        bool init();

        void update_winsize(glm::ivec2& windowSize);
    private:
        enum class EGameState {
            Active,
            Pause
        };

        glm::ivec2 m_windowSize;
        EGameState m_eCurrentGameState;
    };
}
