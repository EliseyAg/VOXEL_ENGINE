class GLFWwindow;

class Window
{
	static GLFWwindow* pWindow;
public:
	static int initialize(int width, int height, const char* title);
	static void finitialize();

	static bool isShouldClose();
	static void PollEvents();
	static void SwapBuffers();
};