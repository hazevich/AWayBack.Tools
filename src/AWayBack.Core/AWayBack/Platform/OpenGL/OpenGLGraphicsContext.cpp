#include "OpenGLGraphicsContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace AWayBack
{
    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow& window)
        : _window(window)
    {

    }

    bool OpenGLGraphicsContext::Initialize()
    {
        glfwMakeContextCurrent(&_window);

        return gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    }

    void OpenGLGraphicsContext::SwapBuffers()
    {
        glfwSwapBuffers(&_window);
    }
}