#include <iostream>
#include "GLFW/glfw3.h"
#include "Rendering/OpenGL/Window.hpp"
#include <memory>

int main()
{
    std::unique_ptr<Rendering::Window> m_Window = std::make_unique<Rendering::Window>("Hello World", 640, 480);

    while (true)
    {
       m_Window->on_update();
    }

    return 0;
}
