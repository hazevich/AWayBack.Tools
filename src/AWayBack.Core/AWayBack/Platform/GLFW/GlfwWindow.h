#pragma once

#include "AWayBack/Core/Window.h"
#include "AWayBack/Platform/OpenGL/OpenGLGraphicsContext.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace AWayBack
{
    class GlfwWindow : public Window
    {
    public:
        GlfwWindow(const std::string& title, uint16_t width, uint16_t height);
        ~GlfwWindow() override;

        uint16_t GetWidth() override { return _data.Width; }
        uint16_t GetHeight() override { return _data.Height; }

        void PollEvents() override;

        GraphicsContext& GetGraphicsContext() override;

        void* GetNativeWindow() override;

    private:
        GLFWwindow* _window;
        OpenGLGraphicsContext* _graphicsContext;

        struct WindowData
        {
            uint16_t Width, Height;
        };

        WindowData _data;
    };
}