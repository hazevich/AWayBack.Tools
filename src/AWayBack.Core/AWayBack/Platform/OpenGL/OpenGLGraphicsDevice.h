#pragma once

#include "AWayBack/Graphics/GraphicsDevice.h"
#include "AWayBack/Graphics/GraphicsContext.h"

namespace AWayBack
{
	class OpenGLGraphicsDevice : public GraphicsDevice
	{
	public:
		OpenGLGraphicsDevice(GraphicsContext& graphicsContext);

		void SwapBuffers() override;

		void SetViewport(uint16_t width, uint16_t height) override;

	private:
		GraphicsContext& _graphicsContext;
	};
}