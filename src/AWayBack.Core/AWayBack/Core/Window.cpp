#include "AWayBack/Core/Window.h"

#ifdef WINDOWS
	#include "AWayBack/Platform/GLFW/GlfwWindow.h"
#endif

namespace AWayBack
{
	Window* Window::Create(std::string title, uint16_t width, uint16_t height)
	{
	#ifdef WINDOWS
			return new GlfwWindow(title, width, height);
	#else
			return nullptr;
	#endif
	}
}