#include "OpenGLGraphicsDevice.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace AWayBack
{
	OpenGLGraphicsDevice::OpenGLGraphicsDevice(GraphicsContext& graphicsContext)
		: _graphicsContext(graphicsContext)
	{
	}
	
	void OpenGLGraphicsDevice::Clear(Color color)
	{
		glClearColor(color.R, color.G, color.B, color.A);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLGraphicsDevice::SwapBuffers()
	{
		_graphicsContext.SwapBuffers();
	}

	void OpenGLGraphicsDevice::SetViewport(uint16_t width, uint16_t height)
	{
		glViewport(0, 0, width, height);
	}
}