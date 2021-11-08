#include "ToolsApplication.h"

#include "imgui.h"

#include "AWayBack/Utils/FileDialog.h"

namespace AWayBack
{
    ToolsApplication::ToolsApplication()
        : _spriteAtlasEditorController(UndoRedoHistory),
          _animatorController(),
          _canvas(_spriteAtlasEditorController),
          _toolbar(_spriteAtlasEditorController, UndoRedoHistory),
          _sprites(_spriteAtlasEditorController),
          _selectedSprite(_spriteAtlasEditorController),
          _newSpriteAtlasModal(_spriteAtlasEditorController),
          _newAnimationAtlasModal(_animatorController),
          _animatorToolbar(_animatorController)
    {
        
    }

    void ToolsApplication::Render()
    {
        RenderDockSpace();
        RenderMainMenuBar();
        
        _newSpriteAtlasModal.Render();
        _newAnimationAtlasModal.Render();

        _canvas.Render();
        _toolbar.Render();
        _animatorToolbar.Render();
        _sprites.Render();
        _selectedSprite.Render();
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
            if (ImGui::MenuItem("Open sprite atlas..."))
            {
                std::optional<std::string> atlasPath = FileDialog::OpenFile("A Way Back Sprite Atlas (*.atlas)\0*.atlas\0");

                if (atlasPath)
                {
                    _spriteAtlasEditorController.LoadSpriteAtlas(atlasPath.value());
                }
            }
            
            if (ImGui::MenuItem("New sprite atlas..."))
            {
                _newSpriteAtlasModal.Open();
            }

            if (ImGui::MenuItem("New animation atlas..."))
            {
                _newAnimationAtlasModal.Open();
            }

            if (ImGui::MenuItem("Exit"))
                Close();
            
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

