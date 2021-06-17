#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "ImGuiOpenGLBackend.h"

namespace AWayBack
{
	ImGuiOpenGLBackend::ImGuiOpenGLBackend(GLFWwindow* window)
		: _window(window)
	{
	}

	void ImGuiOpenGLBackend::Initialize()
	{
		ImGui_ImplGlfw_InitForOpenGL(_window, false);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void ImGuiOpenGLBackend::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
	}

	void ImGuiOpenGLBackend::RenderDrawData()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}