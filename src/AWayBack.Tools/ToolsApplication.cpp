#include "ToolsApplication.h"
#include "imgui.h"
#include "ImGuiExt.h"

namespace AWayBack
{
    ToolsApplication::~ToolsApplication()
    {
        delete _texture;
        delete _newTexture;
    }

    void ToolsApplication::Initialize()
    {
        _openSpriteAtlasModal.IsOpenRequested = true;
    }

    void ToolsApplication::Render()
    {
        UpdateTextures();
        RenderDockSpace();
        RenderMainMenuBar();
        RenderSpriteEditor();
        RenderOpenSpriteAtlasPopup();
    }

    void ToolsApplication::UpdateTextures()
    {
        if (_newTexture)
        {
            delete _texture;
            _texture = _newTexture;
            _newTexture = nullptr;

            _spriteEditor.SetTexture(_texture);
        }
    }

    void ToolsApplication::RenderDockSpace()
    {
        ImGuiViewport* mainViewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowSize(mainViewport->Size);
        ImGui::SetNextWindowPos(mainViewport->Pos);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin(
            "DockSpaceWindow",
            nullptr,
            ImGuiWindowFlags_NoDocking
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus
            | ImGuiWindowFlags_MenuBar
        );

        ImGui::PopStyleVar(3);
        ImGui::DockSpace(ImGui::GetID("DockSpace"));

        ImGui::End();
    }

    void ToolsApplication::RenderMainMenuBar()
    {
        if (!ImGui::BeginMainMenuBar()) return;

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open"))
            {
                _openSpriteAtlasModal.IsOpenRequested = true;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    void ToolsApplication::RenderSpriteEditor()
    {
        _spriteEditor.Render();
    }

    void ToolsApplication::RenderOpenSpriteAtlasPopup()
    {
        if (_openSpriteAtlasModal.Render())
        {
            _newTexture = Texture2D::FromFile(_openSpriteAtlasModal.GetSelectedFile());
        }
    }
}
