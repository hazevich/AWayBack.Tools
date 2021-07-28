#include "ToolbarWindow.h"

#include "imgui_internal.h"

namespace AWayBack
{
    void RenderSpriteAtlasHeader(SpriteEditorController& controller)
    {
        if (!ImGui::CollapsingHeader("Sprite atlas", ImGuiTreeNodeFlags_DefaultOpen)) return;

        const SpriteAtlas& spriteAtlas = controller.GetSpriteAtlas();

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
        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

        ImGuiStyle& style = ImGui::GetStyle();
        
        ImGui::DragInt(
            "##Slice start",
            &controller.SliceStart,
            1,
            0,
            controller.SliceEnd - 1,
            "%d",
            controller.SliceEnd == 1 ? ImGuiSliderFlags_ReadOnly : ImGuiSliderFlags_None
        );
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);
        
        ImGui::DragInt(
            "##Slice end", 
            &controller.SliceEnd, 
            1,
            controller.SliceStart, 
            controller.GridWidth * controller.GridHeight
        );
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        ImGui::Text("Slice range");
    }

    void RenderSlicingControls(SpriteEditorController& controller, bool& isUniformCellSizeControl)
    {
        if (!ImGui::CollapsingHeader("Slicing", ImGuiTreeNodeFlags_DefaultOpen)) return;

        auto slicingTypeInt = (int32_t) controller.SlicingType;
        if (ImGui::Combo("Slicing type", &slicingTypeInt, "Grid sequence\0Grid selection\0Freehand"))
        {
            controller.SlicingType = (SlicingType) slicingTypeInt;
        }

        ImVec2i cellSize = controller.GetCellSize();

        if (ImGui::CellSizeControl(cellSize, isUniformCellSizeControl))
        {
            controller.SetCellSize(cellSize);
        }

        if (controller.SlicingType == SlicingType::GridSequence)
            RenderGridSequenceSlicingControls(controller);

        if (ImGui::Button("Slice"))
        {
            controller.Slice();
        }
    }

    void ToolbarWindow::Render()
    {
        if (!ImGui::Begin("Toolbar"))
        {
            ImGui::End();
            return;
        }

        RenderSpriteAtlasHeader(_controller);
        RenderSlicingControls(_controller, _isUniformCellSizeControl);

        ImGui::End();
    }

}
