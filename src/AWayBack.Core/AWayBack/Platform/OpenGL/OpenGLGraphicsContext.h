#pragma once

#include "AWayBack/Graphics/GraphicsContext.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace AWayBack
{
    class OpenGLGraphicsContext : public GraphicsContext
    {
    public:
        OpenGLGraphicsContext(GLFWwindow& window);

        bool Initialize();

        void SwapBuffers() override;

    private:
        GLFWwindow& _window;
    };
}