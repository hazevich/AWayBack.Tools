#include "imgui.h"
#include "ImGuiRenderer.h"

#include "GLFW/glfw3.h"

namespace AWayBack
{
    ImGuiRenderer::ImGuiRenderer(void* window)
        : _imGuiPlatformBackend(ImGuiPlatformBackend::Create(window))
    {
    }

    ImGuiRenderer::~ImGuiRenderer()
    {
        delete _imGuiPlatformBackend;
        ImGui::DestroyContext();
    }

    void ImGuiRenderer::Initialize()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigWindowsMoveFromTitleBarOnly = true;

        ImGui::StyleColorsDark();

        _imGuiPlatformBackend->Initialize();
    }

    void ImGuiRenderer::NewFrame()
    {
        _imGuiPlatformBackend->NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRenderer::Render()
    {
        _imGuiPlatformBackend->RenderDrawData();

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* currentContextBackup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(currentContextBackup);
        }
    }
}
