#include "GlfwWindow.h"

namespace AWayBack
{
	GlfwWindow::GlfwWindow(const std::string& title, uint16_t width, uint16_t height)
	{
		_data.Width = width;
		_data.Height = height;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		
		glfwMakeContextCurrent(_window);
		glfwSwapInterval(1);
	}

	GlfwWindow::~GlfwWindow()
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void GlfwWindow::Update()
	{
		glfwSwapBuffers(_window);
		glfwPollEvents();

		if (glfwWindowShouldClose(_window))
			Closed.Invoke();
	}
}