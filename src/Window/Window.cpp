#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* Window::pWindow;
unsigned int Window::m_width = 0;
unsigned int Window::m_height = 0;

int Window::initialize(int width, int height, const char* title)
{
	m_width = width;
	m_height = height;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

void Window::setCursorMode(int mode) {
	glfwSetInputMode(pWindow, GLFW_CURSOR, mode);
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
