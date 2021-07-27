#include "ToolsApplication.h"

#include "imgui.h"

#include "AWayBack/Utils/FileDialog.h"
#include "SpriteAtlasSerializer.h"

namespace AWayBack
{
    ToolsApplication::ToolsApplication()
        : _spriteEditorController(UndoRedoHistory),
          _canvas(_spriteEditorController),
          _controls(_spriteEditorController),
          _sprites(_spriteEditorController, UndoRedoHistory),
          _selectedSprite(_spriteEditorController),
          _newSpriteAtlasModal(_spriteEditorController)
    {
        
    }

    void ToolsApplication::Render()
    {
        RenderDockSpace();
        RenderMainMenuBar();
        
        _newSpriteAtlasModal.Render();

        _canvas.Render();
        _controls.Render();
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
            if (ImGui::MenuItem("Open..."))
            {
                std::optional<std::string> atlasPath = FileDialog::OpenFile("A Way Back Sprite Atlas (*.atlas)\0*.atlas\0");

                if (atlasPath)
                {
                    _spriteEditorController.LoadSpriteAtlas(atlasPath.value());
                }
            }
            
            if (ImGui::MenuItem("New..."))
            {
                _newSpriteAtlasModal.Open();
            }

            if (ImGui::MenuItem("Undo"))
                UndoRedoHistory.Undo();

            if (ImGui::MenuItem("Redo"))
                UndoRedoHistory.Redo();

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

