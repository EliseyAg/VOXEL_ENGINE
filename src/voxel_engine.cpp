#include "Window/Window.hpp"

#include <iostream>

int WIDTH = 1280;
int HEIGHT = 720;

int main()
{
	Window::initialize(WIDTH, HEIGHT, "Game");

	while (!Window::isShouldClose())
	{
		Window::PollEvents();
		Window::SwapBuffers();
	}

	Window::finitialize();

	return 0;
}
