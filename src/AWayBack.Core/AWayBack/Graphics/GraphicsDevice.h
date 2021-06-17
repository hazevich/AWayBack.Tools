#pragma once
#include <cstdint>
#include "AWayBack/Graphics/GraphicsContext.h"

namespace AWayBack
{
	class GraphicsDevice
	{
	public:
		virtual void SetViewport(uint16_t width, uint16_t height) = 0;
		virtual void SwapBuffers() = 0;

		static GraphicsDevice* Create(GraphicsContext& graphicsContext);
	};
}