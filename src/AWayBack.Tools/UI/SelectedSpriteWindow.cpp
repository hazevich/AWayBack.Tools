#include "SelectedSpriteWindow.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiExt.h"
#include <cmath>

namespace AWayBack
{
    SelectedSpriteWindow::SelectedSpriteWindow(SpriteAtlasEditorController& controller)
        : _controller(controller)
    {
        
    }

    void RenderOriginDot(ImVec2 cursorPosition)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        drawList->AddCircleFilled(cursorPosition, 3, IM_COL32(51, 250, 137, 255));
    }

    ImVec2 ClampToGrid(ImVec2 vec2, ImVec2i cellSize)
    {
        auto cellXF = (float) cellSize.X;
        auto cellYF = (float) cellSize.Y;

        return ImVec2(std::floor(vec2.x / cellXF) * cellXF, std::floor(vec2.y / cellYF) * cellYF);  
    }

    void RenderGrid(ImVec2 framePosition, ImVec2 imageCursorPos, Vector2 origin, ImVec2i cellSize, ImVec2 contentRegionAvail)
    {        
        auto originCursorPos = ImVec2(imageCursorPos.x + origin.X, imageCursorPos.y + origin.Y);
        auto originPositionClamped = ClampToGrid(originCursorPos, cellSize);

        auto imageOffsetFromGrid = ImVec2(originCursorPos.x - originPositionClamped.x, originCursorPos.y - originPositionClamped.y);

        const float topLeftOffsetMultiplier = 1.5f;
        const float bottomRightOffsetMultiplier = 2.5f;

        auto gridTopLeftOffset = ImVec2(cellSize.X * topLeftOffsetMultiplier, cellSize.Y * topLeftOffsetMultiplier);
        auto gridBottomRightOffset = ImVec2(cellSize.X * bottomRightOffsetMultiplier, cellSize.Y * bottomRightOffsetMultiplier);

        auto gridPosition = ImVec2(framePosition.x + imageOffsetFromGrid.x - gridTopLeftOffset.x, framePosition.y + imageOffsetFromGrid.y - gridTopLeftOffset.y);
        auto gridSize = ImVec2i(contentRegionAvail.x + gridBottomRightOffset.x, contentRegionAvail.y + gridBottomRightOffset.y);

        ImGui::Grid(gridPosition, cellSize, gridSize);
    }

    void RenderSelectionImage(SpriteAtlasEditorController& controller, ImVec2i cellSize, bool isGridVisible)
    {
        ImVec2 framePosition = ImGui::GetCursorScreenPos();

        ImGui::BeginChild("SelectionFrame", ImVec2(0, 0), true);

        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

        if (controller.SelectedSpriteId)
        {
            int32_t selectedSpriteId = controller.SelectedSpriteId.value();
            const Sprite& sprite = controller.GetSprite(selectedSpriteId);
            const Vector2& min = sprite.Min;
            const Vector2& max = sprite.Max;

            Vector2 spriteSize = max - min;
            auto spritePosition = ImVec2((contentRegionAvail.x - spriteSize.X) * 0.5f,
                                         (contentRegionAvail.y - spriteSize.Y) * 0.5f);
            ImGui::SetCursorPos(spritePosition);

            ImGui::CheckerBoard(cellSize, ImVec2i(spriteSize.X, spriteSize.Y));

            Texture2D* texture = controller.GetTexture();

            auto spriteuv0 = ImVec2(min.X / texture->GetWidth(), min.Y / texture->GetHeight());
            auto spriteuv1 = ImVec2(max.X / texture->GetWidth(), max.Y / texture->GetHeight());

            ImVec2 imageScreenPos = ImGui::GetCursorScreenPos();
            ImVec2 imageCursorPos = ImGui::GetCursorPos();

            ImGui::Image(*texture, ImVec2(spriteSize.X, spriteSize.Y), spriteuv0, spriteuv1);

            const Vector2& origin = sprite.Origin;
            auto originScreenPos = ImVec2(imageScreenPos.x + origin.X, imageScreenPos.y + origin.Y);

            RenderOriginDot(originScreenPos);

            if (isGridVisible)
                RenderGrid(framePosition, imageCursorPos, origin, cellSize, contentRegionAvail);
        }

        ImGui::EndChild();
    }

    bool Vector2Control(const char* label, Vector2& vec2, Vector2 min, Vector2 max)
    {
        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

        ImGuiStyle& style = ImGui::GetStyle();

        ImGui::BeginGroup();
        ImGui::PushID(label);

        bool xResult = ImGui::DragFloat("##X", &vec2.X, 1, min.X, max.X);
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        bool yResult = ImGui::DragFloat("##Y", &vec2.Y, 1, min.Y, max.Y);
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        ImGui::PopID();

        ImGui::Text(label);

        ImGui::EndGroup();

        return xResult || yResult;
    }

    void SpriteRegionProperties(int32_t spriteId, Texture2D* texture, SpriteAtlasEditorController& controller)
    {
        if (!ImGui::CollapsingHeader("Sprite region", ImGuiTreeNodeFlags_DefaultOpen)) return;
        const Sprite& sprite = controller.GetSprite(spriteId);
        Vector2 min = sprite.Min;
        Vector2 max = sprite.Max;
        Vector2 spriteSize = max - min;
        
        if (Vector2Control(
            "Position", 
            min, 
            Vector2(0, 0), 
            texture ? Vector2(texture->GetWidth() - spriteSize.X, texture->GetHeight() - spriteSize.Y) : Vector2(0, 0)
        ))
        {
            controller.SetSpritePosition(spriteId, min, false);
        }
        
        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            controller.SetSpritePosition(spriteId, min, true);
        }

        if (Vector2Control(
            "Size", 
            spriteSize, 
            Vector2(1, 1), 
            texture ? Vector2(texture->GetWidth() - min.X, texture->GetHeight() - min.Y) : Vector2(0, 0)
        ))
        {
            controller.SetSpriteSize(spriteId, spriteSize, false);
        }

        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            controller.SetSpriteSize(spriteId, spriteSize, true);
        }
    }

    void RenderOriginControl(int32_t spriteId, SpriteAtlasEditorController& controller)
    {
        auto originPlacementInt = (int32_t) controller.OriginPlacement;

        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

        if (ImGui::Combo(
            "##Origin", 
            &originPlacementInt, 
            "Center\0"
            "Top Left\0"
            "Top Center\0"
            "Top Right\0"
            "Center Right\0"
            "Bottom Right\0"
            "Bottom Center\0"
            "Bottom Left\0"
            "Left Center \0"
            "Custom\0"
        )
            && controller.SelectedSpriteId)
        {
            controller.SetSpriteOrigin(spriteId, (OriginPlacement) originPlacementInt);
        }

        Vector2 origin = controller.GetSprite(spriteId).Origin;

        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        ImGui::BeginGroup();
        ImGui::PushID("Origin");

        bool xResult = ImGui::DragFloat("##X", &origin.X, 1);
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        bool yResult = ImGui::DragFloat("##Y", &origin.Y, 1);
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        ImGui::Text("Origin");

        ImGui::PopID();
        ImGui::EndGroup();

        if (xResult || yResult)
        {
            controller.SetSpriteOrigin(spriteId, origin, false);
            controller.OriginPlacement = OriginPlacement::Custom;
        }

        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            controller.SetSpriteOrigin(spriteId, origin, true);
        }        
    }

    void OriginControls(int32_t spriteId, SpriteAtlasEditorController& controller)
    {
        if (!ImGui::CollapsingHeader("Origin", ImGuiTreeNodeFlags_DefaultOpen)) return;

        if (ImGui::Button("Set for all sprites"))
        {
            const Sprite& sprite = controller.GetSprite(spriteId);

            controller.SetOriginForAllSprites(sprite.Origin);
        }

        RenderOriginControl(spriteId, controller);
    }

    void GridControls(bool& isGridVisible, ImVec2i& cellSize, bool& isUniformCellSizeControl)
    {
        if (!ImGui::CollapsingHeader("Grid", ImGuiTreeNodeFlags_DefaultOpen)) return;

        ImGui::Checkbox("Show", &isGridVisible);
        ImGui::CellSizeControl(cellSize, isUniformCellSizeControl);
    }

    void NameControl(int32_t spriteId, SpriteAtlasEditorController& controller)
    {
        const Sprite& sprite = controller.GetSprite(spriteId);
        const std::string& spriteName = sprite.Name;

        const int32_t SpriteNameMaxSize = 1024;
        char nameBuffer[SpriteNameMaxSize] = { 0 };
        strncpy_s(nameBuffer, spriteName.c_str(), sizeof nameBuffer);

        if (ImGui::InputText("Name", nameBuffer, sizeof nameBuffer, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            controller.RenameSprite(spriteId, nameBuffer);
        }
    }

    void SelectedSpriteWindow::RenderProperties()
    {
        if (!_controller.SelectedSpriteId) return;

        int32_t selectedSpriteId = _controller.SelectedSpriteId.value();
        Texture2D* texture = _controller.GetTexture();

        NameControl(selectedSpriteId, _controller);
        SpriteRegionProperties(selectedSpriteId, texture, _controller);
        OriginControls(selectedSpriteId, _controller);
        GridControls(_isGridVisible, _cellSize, _isUniformCellSizeControl);
    }

    void SelectedSpriteWindow::Render()
    {
        if (!ImGui::Begin("Selection"))
        {
            ImGui::End();
            return;
        }

        RenderProperties();
        RenderSelectionImage(_controller, _cellSize, _isGridVisible);

        ImGui::End();
    }
}