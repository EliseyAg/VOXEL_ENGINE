#pragma once
#include "Window.hpp"

typedef unsigned int uint;

class Events
{
public:
	static bool* _keys;
	static uint* _frames;
	static uint _current_frame;
	static float deltaX;
	static float deltaY;
	static float x;
	static float y;
	static bool _cursor_locked;
	static bool _cursor_started;

	static int initialize();
	static void finitialize();

	static void pollEvents();

	static bool pressed(int keycode);
	static bool jpressed(int keycode);

	static bool clicked(int button);
	static bool jclicked(int button);
};
