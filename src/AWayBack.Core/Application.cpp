#include <stdio.h>
#include <iostream>
#include "Application.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "AWayBack/Core/Window.h"
#include <functional>

namespace AWayBack
{
	void Application::Run()
	{
		Window* window = Window::Create("A Way Back Tools", 800, 600);

		window->Closed += std::bind(&Application::OnClose, this);

		while(_isRunning)
		{
			window->Update();
		}

		delete window;
	}

	void Application::OnClose()
	{
		_isRunning = false;
	}
}
