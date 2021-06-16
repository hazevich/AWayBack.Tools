#pragma once
#include <cstdint>

namespace AWayBack
{
	class GraphicsDevice
	{
	public:
		virtual void SetViewport(uint16_t width, uint16_t height) = 0;

		static GraphicsDevice* Create();
	};
}