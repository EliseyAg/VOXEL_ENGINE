#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* Window::pWindow;

int Window::initialize(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (pWindow == nullptr)
	{
		std::cerr << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);
	return 0;
}

void Window::finitialize()
{
	glfwTerminate();
}

bool Window::isShouldClose()
{
	return glfwWindowShouldClose(pWindow);
}

void Window::setShouldClose(bool flag) {
	glfwSetWindowShouldClose(pWindow, flag);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(pWindow);
}
