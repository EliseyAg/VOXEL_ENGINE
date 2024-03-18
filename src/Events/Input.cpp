#include "Input.hpp"

#define MOUSE_BUTTONS 1024

namespace Events
{
	bool Input::m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
	bool Input::m_mouse_buttons_pressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};
	uint*Input::m_frames = new uint[1032];
	uint Input::m_current = 0;

	void Input::update()
	{
		m_current++;
	}

	bool Input::IsKeyPressed(const KeyCode key_code)
	{
		return m_keys_pressed[static_cast<size_t>(key_code)];
	}

	bool Input::IsKeyJustPressed(const KeyCode key_code)
	{
		return m_keys_pressed[static_cast<size_t>(key_code)] && m_frames[static_cast<size_t>(key_code)] == m_current;
	}
	
	void Input::PressKey(const KeyCode key_code)
	{
		m_keys_pressed[static_cast<size_t>(key_code)] = true;
		m_frames[static_cast<size_t>(key_code)] = m_current;
	}

	void Input::ReleaseKey(const KeyCode key_code)
	{
		m_keys_pressed[static_cast<size_t>(key_code)] = false;
		m_frames[static_cast<size_t>(key_code)] = m_current;
	}

	bool Input::IsMouseButtonPressed(const MouseButton mouse_button)
	{
		return m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)];
	}

	bool Input::IsMouseButtonJustPressed(const MouseButton mouse_button)
	{
		return m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] && m_frames[MOUSE_BUTTONS + static_cast<size_t>(mouse_button)] == m_current;
	}

	void Input::PressMouseButton(const MouseButton mouse_button)
	{
		m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = true;
		m_frames[MOUSE_BUTTONS + static_cast<size_t>(mouse_button)] = m_current;
	}

	void Input::ReleaseMouseButton(const MouseButton mouse_button)
	{
		m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = false;
		m_frames[MOUSE_BUTTONS + static_cast<size_t>(mouse_button)] = m_current;
	}
}