#pragma once
#include <cstdint>
#include <string>
#include "Event.h"

namespace AWayBack
{
	class Window
	{
	public:
		virtual ~Window() = default;

		Event<void> Closed;

		virtual uint16_t GetWidth() = 0;
		virtual uint16_t GetHeight() = 0;
		virtual void Update() = 0;

		static Window* Create(std::string title, uint16_t width, uint16_t height);
	};
}