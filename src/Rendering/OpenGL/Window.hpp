#pragma once
#include <string>

class GLFWwindow;

namespace Rendering
{
	class Window
	{
	public:
		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void on_update();

	private:
		struct WindowData
		{
			std::string title;
			unsigned int width;
			unsigned int height;
		};

		int init();
		void shutdown();

		GLFWwindow* m_pWindow = nullptr;
		WindowData m_data;
	};
}
