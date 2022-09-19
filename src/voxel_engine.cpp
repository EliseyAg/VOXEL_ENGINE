#include <iostream>

#include <GLFW/glfw3.h>

int WIDTH = 1280;
int HEIGHT = 720;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* pWindow = glfwCreateWindow(WIDTH, HEIGHT, "Window", nullptr, nullptr);
	if (pWindow == nullptr)
	{
		std::cerr << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);

	//glViewport(0, 0, WIDTH, HEIGHT);

	while (!glfwWindowShouldClose(pWindow))
	{
		glfwPollEvents();
		glfwSwapBuffers(pWindow);
	}

	glfwTerminate();

	return 0;
}
