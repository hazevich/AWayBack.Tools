#pragma once

#include "AWayBack/Graphics/GraphicsDevice.h"

namespace AWayBack
{
	class OpenGLGraphicsDevice : public GraphicsDevice
	{
	public:
		void SetViewport(uint16_t width, uint16_t height) override;
	};
}