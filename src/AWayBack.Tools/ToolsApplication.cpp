#include "ToolsApplication.h"

#include "imgui.h"

#include "AWayBack/Utils/FileDialog.h"
#include "SpriteAtlasSerializer.h"

namespace AWayBack
{
    void ToolsApplication::Initialize()
    {
        _openSpriteAtlasModal.IsOpenRequested = true;
    }

    void ToolsApplication::Render()
    {
        RenderDockSpace();
        RenderMainMenuBar();
        RenderSpriteEditor();
        RenderOpenSpriteAtlasPopup();
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
            if (ImGui::MenuItem("Open..."))
            {
                std::optional<std::string> atlasPath = FileDialog::OpenFile("A Way Back Sprite Atlas (*.atlas)\0*.atlas\0");

                if (atlasPath)
                {
                    _spriteEditor.LoadSpriteAtlas(atlasPath.value());
                }
            }

            if (ImGui::MenuItem("Browse"))
            {
                std::optional<std::string> folder = FileDialog::OpenFolder();
                printf(folder.value_or("none").c_str());
            }

            if (ImGui::MenuItem("New"))
            {
                _spriteEditor.CreateNewSpriteAtlas();
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
            _spriteEditor.LoadTexture(_openSpriteAtlasModal.GetSelectedFile());
        }
    }
}
