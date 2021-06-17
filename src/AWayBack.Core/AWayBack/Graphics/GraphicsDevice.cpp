#include "GraphicsDevice.h"

#ifdef WINDOWS
    #include "AWayBack/Platform/OpenGL/OpenGLGraphicsDevice.h"
#endif // WINDOWS

namespace AWayBack
{
    GraphicsDevice* GraphicsDevice::Create(GraphicsContext& graphicsContext)
    {
    #ifdef WINDOWS
        return new OpenGLGraphicsDevice(graphicsContext);
    #else
        return nullptr;
    #endif // WINDOWS

    }
}