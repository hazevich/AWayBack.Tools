#include <stdio.h>
#include <iostream>
#include "Application.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "AWayBack/Core/Window.h"
#include "AWayBack/Graphics/GraphicsDevice.h"
#include <functional>

namespace AWayBack
{
	void Application::Run()
	{
		Window* window = Window::Create("A Way Back Tools", 800, 600);
		_graphicsDevice = GraphicsDevice::Create(window->GetGraphicsContext());

		window->Closed += std::bind(&Application::OnClose, this);
		window->Resized += std::bind(&Application::OnWindowResized, this, std::placeholders::_1);

		while(_isRunning)
		{
			window->PollEvents();

			_graphicsDevice->Clear(Color(242, 208, 107));
			_graphicsDevice->SwapBuffers();
		}

		delete window;
	}

	void Application::OnClose()
	{
		_isRunning = false;
	}

	void Application::OnWindowResized(const WindowResizedData& data)
	{
		_graphicsDevice->SetViewport(data.Width, data.Height);
	}
}
