#include "Window/Window.hpp"
#include "Window/Events.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Graphics/Texture.hpp"
#include "Camera/Camera.hpp"
#include "Loaders/png_loader.hpp"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

int WIDTH = 1280;
int HEIGHT = 720;

float vertices[] = {
	// x    y     z     u     v
   -1.0f,-1.0f, 0.0f, 0.0f, 0.0f,
	1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

int main(int argc, char** argv)
{
	std::string executablePath = argv[0];
	size_t found = executablePath.find_last_of("/\\");
	std::string m_path = executablePath.substr(0, found);

	int returnCode = Window::initialize(WIDTH, HEIGHT, "Game");
	Events::initialize();
	if (!gladLoadGL())
	{
		std::cout << "Can't load GLAD!" << std::endl;
	}

	ShaderProgram* shader = load_shader(m_path + "/" + "res/shaders/main.glslv", m_path + "/" + "res/shaders/main.glslf");
	if (shader == nullptr) {
		std::cerr << "failed to load shader program" << std::endl;
		Window::finitialize();
		return 1;
	}

	Texture* texture = load_texture(m_path + "/" + "res/textures/img.png");
	if (texture == nullptr) {
		std::cerr << "failed to load texture" << std::endl;
		delete shader;
		Window::finitialize();
		return 1;
	}

	// Create VAO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glClearColor(0.6f, 0.62f, 0.65f, 1);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera* camera = new Camera(glm::vec3(0, 0, 1), glm::radians(60.f));

	glm::mat4 model(1.0f);

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	float speed = 5;

	float camX = 0;
	float camY = 0;

	while (!Window::isShouldClose())
	{
		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;

		lastTime = currentTime;

		if (Events::jpressed(GLFW_KEY_ESCAPE))
		{
			Window::setShouldClose(true);
		}
		if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) 
		{
			glClearColor(0.8f, 0.4f, 0.2f, 1);
		}
		if (Events::pressed(GLFW_KEY_TAB))
		{
			Events::toogleCursor();
		}
		if (Events::pressed(GLFW_KEY_W))
		{
			camera->position += camera->front * delta * speed;
		}
		if (Events::pressed(GLFW_KEY_S))
		{
			camera->position -= camera->front * delta * speed;
		}
		if (Events::pressed(GLFW_KEY_A))
		{
			camera->position -= camera->right * delta * speed;
		}
		if (Events::pressed(GLFW_KEY_D))
		{
			camera->position += camera->right * delta * speed;
		}
		camY += -Events::deltaY / Window::m_height;
		camX += -Events::deltaX / Window::m_height;

		if (camY < -glm::radians(89.0f))
		{
			camY = -glm::radians(89.0f);
		}
		if (camY > glm::radians(89.0f))
		{
			camY = glm::radians(89.0f);
		}

		camera->rotation = glm::mat4(1.0f);
		camera->rotate(camY, camX, 0);

		glClear(GL_COLOR_BUFFER_BIT);

		shader->bind();
		shader->setMat4("u_model", model);
		shader->setMat4("u_projview", camera->getProjection() * camera->getView());
		texture->bind();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		Window::swapBuffers();
		Events::pollEvents();
	}

	delete shader;
	delete texture;
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	Window::finitialize();

	return returnCode;
}
