#pragma once

#include "AWayBack/Graphics/GraphicsDevice.h"
#include "AWayBack/Core/Window.h"

namespace AWayBack
{
	class Application
	{
	public:
		void Run();
	private:
		void OnClose();
		void OnWindowResized(const WindowResizedData& data);
	private:
		bool _isRunning = true;
		GraphicsDevice* _graphicsDevice;
	};
}