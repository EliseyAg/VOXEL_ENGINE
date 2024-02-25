#include <iostream>
#include "Rendering/OpenGL/Window.hpp"
#include "Rendering/OpenGL/ShaderProgram.hpp"
#include "Rendering/OpenGL/Texture2D.hpp"
#include "Resources/ResourceManager.hpp"
#include "Events/Event.hpp"
#include <memory>
#include "glad/glad.h"


GLfloat point[] = {
 0.0f,  0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
-0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

GLuint vao;

int main(int argc, char** argv)
{
    Resources::ResourceManager::init(argv[0]);

    std::unique_ptr<Rendering::Window> m_pWindow = std::make_unique<Rendering::Window>("Hello World", 640, 480);

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
            glViewport(0, 0, event.width, event.height);
        }
    );

    m_pWindow->set_event_callback(
        [&](Events::BaseEvent& event)
        {
            m_event_dispatcher.dispatch(event);
        }
    );

    auto pDefaultShaderProgram = Resources::ResourceManager::loadShaderProgram("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
        return -1;
    }

    auto tex = Resources::ResourceManager::loadTexture("DefaultTexture", "res/textures/Blocks.png");

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint texCoord_vbo = 0;
    glGenBuffers(1, &texCoord_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);


    pDefaultShaderProgram->bind();
    pDefaultShaderProgram->setInt("tex", 0);


    while (!m_bCloseWindow)
    {
       glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
       glClear(GL_COLOR_BUFFER_BIT);

       pDefaultShaderProgram->bind();
       glBindVertexArray(vao);
       tex->bind();
       glDrawArrays(GL_TRIANGLES, 0, 3);

       m_pWindow->on_update();

    }

    m_pWindow = nullptr;

    return 0;
}
