#pragma once

#include "AWayBack/Platform/GLFW/GlfwWindow.h"

namespace AWayBack
{
    class ImGuiRenderer
    {
    public:
        ImGuiRenderer(void* window);
        ~ImGuiRenderer();

        void Initialize();
        void NewFrame();
        void Render();

    private:
        GLFWwindow* _window;
    };
}
