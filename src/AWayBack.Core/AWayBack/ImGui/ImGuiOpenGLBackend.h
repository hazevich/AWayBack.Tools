#pragma once

#include "ImGuiPlatformBackend.h"
#include "GLFW/glfw3.h"

namespace AWayBack
{
    class ImGuiOpenGLBackend : public ImGuiPlatformBackend
    {
    public:
        ImGuiOpenGLBackend(GLFWwindow* window);

        void Initialize() override;
        void NewFrame() override;
        void RenderDrawData() override;

    private:
        GLFWwindow* _window;
    };
}