#include "imgui.h"
#include "ImGuiRenderer.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

namespace AWayBack
{
    ImGuiRenderer::ImGuiRenderer(void* window)
        : _window(static_cast<GLFWwindow*>(window))
    {
    }

    ImGuiRenderer::~ImGuiRenderer()
    {
    #ifdef WINDOWS
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    #endif // WINDOWS

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
        
    #ifdef WINDOWS
        ImGui_ImplGlfw_InitForOpenGL(_window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    #endif //WINDOWS
    }

    void ImGuiRenderer::NewFrame()
    {
    #ifdef WINDOWS
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    #endif // WINDOWS

        ImGui::NewFrame();
    }

    void ImGuiRenderer::Render()
    {
        ImGui::Render();
    #ifdef WINDOWS
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    #endif // WINDOWS
    }
}
