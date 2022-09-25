#pragma once

class GLFWwindow;

class Window
{
public:
	static int initialize(int width, int height, const char* title);
	static void finitialize();

	static void setCursorMode(int mode);
	static bool isShouldClose();
	static void setShouldClose(bool flag);
	static void swapBuffers();

	static GLFWwindow* pWindow;
	static unsigned int m_width;
	static unsigned int m_height;
};