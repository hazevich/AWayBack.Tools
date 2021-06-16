#include "GraphicsDevice.h"

#ifdef WINDOWS
	#include "AWayBack/OpenGL/OpenGLGraphicsDevice.h"
#endif // WINDOWS

namespace AWayBack
{
	GraphicsDevice* GraphicsDevice::Create()
	{
	#ifdef WINDOWS
		return new OpenGLGraphicsDevice();
	#else
		return nullptr;
	#endif // WINDOWS

	}
}