#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "ImGuiRenderer.h"

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
        ImGui::Render();
        _imGuiPlatformBackend->RenderDrawData();
    }
}