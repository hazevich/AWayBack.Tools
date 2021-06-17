#pragma once
#include <cstdint>
#include <string>
#include "Event.h"
#include "AWayBack/Graphics/GraphicsContext.h"

namespace AWayBack
{
	struct WindowResizedData
	{
		const uint16_t Width, Height;

		WindowResizedData(uint16_t width, uint16_t height)
			: Width{width}, Height{height}
		{
		}
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		Event<void> Closed;
		Event<WindowResizedData> Resized;

		virtual uint16_t GetWidth() = 0;
		virtual uint16_t GetHeight() = 0;
		virtual void PollEvents() = 0;
		virtual GraphicsContext& GetGraphicsContext() = 0;

		static Window* Create(std::string title, uint16_t width, uint16_t height);
	};
}