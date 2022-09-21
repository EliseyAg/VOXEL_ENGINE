#include "Window/Window.hpp"
#include "Window/Events.hpp"

#include <iostream>
#include <GLFW/glfw3.h>

int WIDTH = 1280;
int HEIGHT = 720;

int main()
{
	int returnCode = Window::initialize(WIDTH, HEIGHT, "Game");
	Events::initialize();

	while (!Window::isShouldClose())
	{
		Events::pollEvents();
		if (Events::jpressed(GLFW_KEY_ESCAPE)) {
			Window::setShouldClose(true);
		}
		Window::swapBuffers();
	}

	Window::finitialize();

	return returnCode;
}
