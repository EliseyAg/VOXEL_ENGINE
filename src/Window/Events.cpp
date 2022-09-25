#include "Events.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>

bool* Events::_keys;
uint* Events::_frames;
uint  Events::_current_frame = 0;
float Events::deltaX;
float Events::deltaY;
float Events::x;
float Events::y;
bool Events::_cursor_locked = false;
bool Events::_cursor_started = false;

#define _MOUSE_BUTTONS 1024

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (Events::_cursor_started) {
		Events::deltaX += xpos - Events::x;
		Events::deltaY += ypos - Events::y;
	}
	else {
		Events::_cursor_started = true;
	}
	Events::x = xpos;
	Events::y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
	if (action == GLFW_PRESS) {
		Events::_keys[_MOUSE_BUTTONS + button] = true;
		Events::_frames[_MOUSE_BUTTONS + button] = Events::_current_frame;
	}
	else if (action == GLFW_RELEASE) {
		Events::_keys[_MOUSE_BUTTONS + button] = false;
		Events::_frames[_MOUSE_BUTTONS + button] = Events::_current_frame;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		Events::_keys[key] = true;
		Events::_frames[key] = Events::_current_frame;
	}
	else if (action == GLFW_RELEASE) {
		Events::_keys[key] = false;
		Events::_frames[key] = Events::_current_frame;
	}
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Window::m_width = width;
	Window::m_height = height;
}

int Events::initialize()
{
	GLFWwindow* pWindow = Window::pWindow;
	_keys = new bool[1032];
	_frames = new uint[1032];

	memset(_keys, false, 1032*sizeof(bool));
	memset(_frames, 0, 1032*sizeof(uint));

	glfwSetKeyCallback(pWindow, key_callback);
	glfwSetMouseButtonCallback(pWindow, mouse_button_callback);
	glfwSetCursorPosCallback(pWindow, cursor_position_callback);
	glfwSetWindowSizeCallback(pWindow, window_size_callback);

	return 0;
}

void Events::finitialize()
{

}

bool Events::pressed(int keycode) {
	if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
		return false;
	return _keys[keycode];
}

bool Events::jpressed(int keycode) {
	if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
		return false;
	return _keys[keycode] && _frames[keycode] == _current_frame;
}

bool Events::clicked(int button) {
	int index = _MOUSE_BUTTONS + button;
	return _keys[index];
}

bool Events::jclicked(int button) {
	int index = _MOUSE_BUTTONS + button;
	return _keys[index] && _frames[index] == _current_frame;
}

void Events::toogleCursor() {
	_cursor_locked = !_cursor_locked;
	Window::setCursorMode(_cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Events::pollEvents()
{
	_current_frame++;
	deltaX = 0.0f;
	deltaY = 0.0f;
	glfwPollEvents();
}
