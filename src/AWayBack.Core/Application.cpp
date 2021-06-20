#include <stdio.h>
#include <iostream>
#include "Application.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "AWayBack/Core/Window.h"
#include "AWayBack/Graphics/GraphicsDevice.h"
#include "AWayBack/ImGui/ImGuiRenderer.h"
#include <functional>
#include "imgui.h"
#include "AWayBack/Graphics/Texture.h"
#include "filesystem"

namespace AWayBack
{
    void Application::Run()
    {
        Window* window = Window::Create("A Way Back Tools", 800, 600);
        _graphicsDevice = GraphicsDevice::Create(window->GetGraphicsContext());

        window->Resized += std::bind(&Application::OnWindowResized, this, std::placeholders::_1);
        window->Closed += std::bind(&Application::OnClose, this);

        ImGuiRenderer* imGuiRenderer = new ImGuiRenderer(window->GetNativeWindow());
        imGuiRenderer->Initialize();

        Initialize();
        
        Texture2D* fatKitty = Texture2D::FromFile("fatcat.png");

        while (_isRunning)
        {
            window->PollEvents();
            imGuiRenderer->NewFrame();
            
            Render();

            ImGui::Render();
            _graphicsDevice->Clear(Color(242, 208, 107));
            imGuiRenderer->Render();

            _graphicsDevice->SwapBuffers();
        }

        delete fatKitty;
        delete imGuiRenderer;
        delete _graphicsDevice;
        delete window;
    }

    void Application::OnClose()
    {
        _isRunning = false;
    }

    void Application::OnWindowResized(const WindowResizedData& data)
    {
        _graphicsDevice->SetViewport(data.Width, data.Height);
    }
}
