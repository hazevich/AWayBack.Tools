#include "SpriteEditorControlsWindow.h"

#include "imgui_internal.h"

namespace AWayBack
{
    void RenderSpriteAtlasHeader(SpriteEditorController& controller)
    {
        if (!ImGui::CollapsingHeader("Sprite atlas", ImGuiTreeNodeFlags_DefaultOpen)) return;

        SpriteAtlas& spriteAtlas = controller.GetSpriteAtlas();

        ImGui::LabelText("Sprite Atlas Name", spriteAtlas.Name.c_str());
        ImGui::NewLine();
        ImGui::LabelText("Texture name", spriteAtlas.TextureName.c_str());

        if (ImGui::Button("Save"))
        {
            controller.Save();
        }
    }

    void RenderGridSequenceSlicingControls(SpriteEditorController& controller)
    {
        ImGui::DragInt(
            "Slice start",
            &controller.SliceStart,
            1,
            0,
            controller.SliceEnd - 1,
            "%d",
            controller.SliceEnd == 1 ? ImGuiSliderFlags_ReadOnly : ImGuiSliderFlags_None
        );
        ImGui::DragInt(
            "Slice end", 
            &controller.SliceEnd, 
            1,
            controller.SliceStart, 
            controller.GridWidth * controller.GridHeight
        );
    }

    void RenderSlicingControls(SpriteEditorController& controller, bool& isUniformCellSizeControl)
    {
        if (!ImGui::CollapsingHeader("Slicing", ImGuiTreeNodeFlags_DefaultOpen)) return;

        auto slicingTypeInt = (int32_t) controller.SlicingType;
        if (ImGui::Combo("Slicing type", &slicingTypeInt, "Grid sequence\0Grid selection\0Freehand"))
        {
            controller.SlicingType = (SlicingType) slicingTypeInt;
        }

        ImGui::ImVec2i cellSize = controller.GetCellSize();

        if (isUniformCellSizeControl)
        {
            if (ImGui::DragInt("##Cell size", &cellSize.X, 1, 1, INT32_MAX))
            {
                cellSize.Y = cellSize.X;

                controller.SetCellSize(cellSize);
            }
        }
        else if (ImGui::DragInt2("##Cell size", cellSize.Components, 1, 1, INT32_MAX))
        {
            controller.SetCellSize(cellSize);
        }

        ImGui::SameLine();

        if (ImGui::Button("Cell size"))
        {
            isUniformCellSizeControl = !isUniformCellSizeControl;
        }

        if (controller.SlicingType == SlicingType::GridSequence)
            RenderGridSequenceSlicingControls(controller);

        if (ImGui::Button("Slice"))
        {
            controller.Slice();
        }
    }

    void SpriteEditorControlsWindow::Render()
    {
        if (!ImGui::Begin("Controls"))
        {
            ImGui::End();
            return;
        }

        RenderSpriteAtlasHeader(_controller);
        RenderSlicingControls(_controller, _isUniformCellSizeControl);

        ImGui::End();
    }

}
