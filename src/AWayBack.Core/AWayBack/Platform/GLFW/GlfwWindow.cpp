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
		
		_graphicsContext = new OpenGLGraphicsContext(*_window);
		_graphicsContext->Initialize();

		glfwSwapInterval(1);

		glfwSetWindowUserPointer(_window, this);

		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height)
			{
				WindowResizedData windowResizedData{(uint16_t) width, (uint16_t) height};

				auto thisWindow = (GlfwWindow*) glfwGetWindowUserPointer(window);
				thisWindow->Resized.Invoke(windowResizedData);

				thisWindow->_data.Width = width;
				thisWindow->_data.Height = height;
			}
		);

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
			{
				auto thisWindow = (GlfwWindow*) glfwGetWindowUserPointer(window);
				thisWindow->Closed.Invoke();
			}
		);
	}

	GlfwWindow::~GlfwWindow()
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	GraphicsContext& GlfwWindow::GetGraphicsContext()
	{
		return *_graphicsContext;
	}

	void GlfwWindow::PollEvents()
	{
		glfwPollEvents();
	}
}