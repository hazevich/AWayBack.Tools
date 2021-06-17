#include "ImGuiPlatformBackend.h"

#ifdef WINDOWS
    #include "ImGuiOpenGLBackend.h"
#endif // WINDOWS

namespace AWayBack
{
    ImGuiPlatformBackend* ImGuiPlatformBackend::Create(void* window)
    {
    #ifdef WINDOWS
        return new ImGuiOpenGLBackend((GLFWwindow*)window);
    #else
        return nullptr;
    #endif // WINDOWS
    }
}