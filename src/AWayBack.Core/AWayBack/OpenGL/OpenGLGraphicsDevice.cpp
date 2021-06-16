#include "OpenGLGraphicsDevice.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace AWayBack
{
	void OpenGLGraphicsDevice::SetViewport(uint16_t width, uint16_t height)
	{
		glViewport(0, 0, width, height);
	}
}