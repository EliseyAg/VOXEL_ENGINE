#pragma once

class GLFWwindow;

class Window
{
public:
	static int initialize(int width, int height, const char* title);
	static void finitialize();

	static bool isShouldClose();
	static void setShouldClose(bool flag);
	static void swapBuffers();

	static GLFWwindow* pWindow;
};