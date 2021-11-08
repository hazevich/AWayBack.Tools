#include "ToolbarWindow.h"

#include "imgui_internal.h"

namespace AWayBack
{
    void RenderSpriteAtlasHeader(SpriteAtlasEditorController& controller)
    {
        if (!ImGui::CollapsingHeader("Sprite atlas", ImGuiTreeNodeFlags_DefaultOpen)) return;

        const SpriteAtlas* spriteAtlas = controller.GetSpriteAtlas();

        ImGui::LabelText("Sprite Atlas Name", spriteAtlas ? spriteAtlas->Name.c_str() : "");
        ImGui::NewLine();
        ImGui::LabelText("Texture name", spriteAtlas ? spriteAtlas->Texture->GetName().c_str() : "");
    }

    void RenderGridSequenceSlicingControls(SpriteAtlasEditorController& controller)
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

    void RenderSlicingControls(SpriteAtlasEditorController& controller, bool& isUniformCellSizeControl)
    {
        if (!ImGui::CollapsingHeader("Slicing", ImGuiTreeNodeFlags_DefaultOpen)) return;

        auto slicingTypeInt = (int32_t) controller.SlicingType;

        if (ImGui::Combo("Slicing type", &slicingTypeInt, "Grid sequence\0Grid selection\0Freehand"))
            controller.SlicingType = (SlicingType) slicingTypeInt;

        ImGui::Checkbox("Show canvas grid", &controller.IsCanvasGridVisible);

        ImVec2i cellSize = controller.GetCellSize();

        if (ImGui::CellSizeControl(cellSize, isUniformCellSizeControl))
            controller.SetCellSize(cellSize);

        if (controller.SlicingType == SlicingType::GridSequence)
            RenderGridSequenceSlicingControls(controller);
    }

    void RenderGeneralControls(UndoRedoHistory& undoRedoHistory, SpriteAtlasEditorController& controller)
    {
        const ImVec2 buttonSize = ImVec2(50, 50);

        if (ImGui::CustomButton("Undo", buttonSize, undoRedoHistory.CanUndo()))
            undoRedoHistory.Undo();

        ImGui::SameLine();
        
        if (ImGui::CustomButton("Redo", buttonSize, undoRedoHistory.CanRedo()))
            undoRedoHistory.Redo();

        ImGui::SameLine();

        if (ImGui::Button("Save", buttonSize))
            controller.Save();

        ImGui::SameLine();

        if (ImGui::CustomButton("Slice", buttonSize, controller.CanSlice()))
            controller.Slice();
    }

    void ToolbarWindow::Render()
    {
        if (!ImGui::Begin("Toolbar"))
        {
            ImGui::End();
            return;
        }

        RenderGeneralControls(_undoRedoHistory, _controller);
        RenderSpriteAtlasHeader(_controller);
        RenderSlicingControls(_controller, _isUniformCellSizeControl);

        ImGui::End();
    }
    
    namespace UI::AnimationEditor
    {
        AnimatorToolbarWindow::AnimatorToolbarWindow(AnimatorController& animatorController)
            : _animatorController(animatorController)
        {
            
        }

        void AnimatorToolbarWindow::Render()
        {
            if (!ImGui::Begin("Animator toolbar"))
            {
                ImGui::End();
                return;
            }
            const AnimationAtlas* animationAtlas = _animatorController.GetAnimationAtlas();

            if (animationAtlas)
            {
                ImGui::LabelText("Animation atlas", animationAtlas->Name.c_str());
                ImGui::LabelText("Sprite atlas", animationAtlas->SpriteAtlas->Name.c_str());
            }

            ImGui::End();
        }
    }
}
