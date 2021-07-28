#include "SelectedSpriteWindow.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiExt.h"

#include "UndoRedo.h"

#include <cmath>

namespace AWayBack
{
    SelectedSpriteWindow::SelectedSpriteWindow(SpriteEditorController& controller)
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

    void RenderSelectionImage(SpriteEditorController& controller, ImVec2i cellSize, bool isGridVisible)
    {
        ImVec2 framePosition = ImGui::GetCursorScreenPos();

        ImGui::BeginChild("SelectionFrame", ImVec2(0, 0), true);

        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

        if (controller.SelectedSpriteId)
        {
            SpriteAtlas& spriteAtlas = controller.GetSpriteAtlas();
            Sprite& sprite = spriteAtlas.Sprites[controller.SelectedSpriteId.value()];
            Vector2 spriteSize = sprite.Max - sprite.Min;
            auto spritePosition = ImVec2((contentRegionAvail.x - spriteSize.X) * 0.5f,
                                         (contentRegionAvail.y - spriteSize.Y) * 0.5f);
            ImGui::SetCursorPos(spritePosition);

            ImGui::CheckerBoard(cellSize, ImVec2i(spriteSize.X, spriteSize.Y));

            Texture2D* texture = controller.GetTexture();

            auto spriteuv0 = ImVec2(sprite.Min.X / texture->GetWidth(), sprite.Min.Y / texture->GetHeight());
            auto spriteuv1 = ImVec2(sprite.Max.X / texture->GetWidth(), sprite.Max.Y / texture->GetHeight());

            ImVec2 imageScreenPos = ImGui::GetCursorScreenPos();
            ImVec2 imageCursorPos = ImGui::GetCursorPos();

            ImGui::Image(*texture, ImVec2(spriteSize.X, spriteSize.Y), spriteuv0, spriteuv1);
            
            auto originScreenPos = ImVec2(imageScreenPos.x + sprite.Origin.X, imageScreenPos.y + sprite.Origin.Y);

            RenderOriginDot(originScreenPos);

            if (isGridVisible)
                RenderGrid(framePosition, imageCursorPos, sprite.Origin, cellSize, contentRegionAvail);
        }

        ImGui::EndChild();
    }

    bool Vector2Control(const char* label, Vector2& vec2, Vector2 min, Vector2 max)
    {
        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

        ImGuiStyle& style = ImGui::GetStyle();

        ImGui::PushID(label);

        bool xResult = ImGui::DragFloat("##X", &vec2.X, 1, min.X, max.X);
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        bool yResult = ImGui::DragFloat("##Y", &vec2.Y, 1, min.Y, max.Y);
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        ImGui::PopID();

        ImGui::Text(label);

        return xResult || yResult;
    }

    void SpriteRegionControls(Sprite& sprite, Texture2D* texture)
    {
        if (!ImGui::CollapsingHeader("Sprite region", ImGuiTreeNodeFlags_DefaultOpen)) return;

        Vector2 spriteSize = sprite.Max - sprite.Min;
        
        if (Vector2Control(
            "Position", 
            sprite.Min, 
            Vector2(0, 0), 
            texture ? Vector2(texture->GetWidth() - spriteSize.X, texture->GetHeight() - spriteSize.Y) : Vector2(0, 0)
        ))
        {
            sprite.Max = sprite.Min + spriteSize;
        }

        if (Vector2Control(
            "Size", 
            spriteSize, 
            Vector2(1, 1), 
            texture ? Vector2(texture->GetWidth() - sprite.Min.X, texture->GetHeight() - sprite.Min.Y) : Vector2(0, 0)
        ))
        {
            sprite.Max = sprite.Min + spriteSize;
        }
    }

    Vector2 CalculateOrigin(Sprite& sprite, OriginPlacement originPlacement)
    {
        Vector2 spriteSize = sprite.Max - sprite.Min;

        switch (originPlacement)
        {
        case Center:
            return Vector2(spriteSize.X / 2, spriteSize.Y / 2);
        case TopLeft:
            return Vector2(0, 0);
        case TopCenter:
            return Vector2(spriteSize.X / 2, 0);
        case TopRight:
            return Vector2(spriteSize.X, 0);
        case CenterRight:
            return Vector2(spriteSize.X, spriteSize.Y / 2);
        case BottomRight:
            return spriteSize;
        case BottomCenter:
            return Vector2(spriteSize.X / 2, spriteSize.Y);
        case BottomLeft:
            return Vector2(0, spriteSize.Y);
        case LeftCenter:
            return Vector2(0, spriteSize.Y / 2);
        case Custom:
            return sprite.Origin;
        }

        return sprite.Origin;
    }

    void RenderOriginControl(Sprite& sprite, OriginPlacement& originPlacement)
    {
        auto originPlacementInt = (int32_t*) &originPlacement;

        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

        if (ImGui::Combo(
            "##Origin", 
            originPlacementInt, 
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
        ))
        {
            sprite.Origin = CalculateOrigin(sprite, originPlacement);
        }
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);
        
        ImGui::PushID("Origin");

        bool xResult = ImGui::DragFloat("##X", &sprite.Origin.X, 1);
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        bool yResult = ImGui::DragFloat("##Y", &sprite.Origin.Y, 1);
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        ImGui::PopID();

        ImGui::Text("Origin");

        if (xResult || yResult)
            originPlacement = OriginPlacement::Custom;
    }

    void OriginControls(Sprite& sprite, SpriteAtlas& spriteAtlas, OriginPlacement& originPlacement)
    {
        if (!ImGui::CollapsingHeader("Origin", ImGuiTreeNodeFlags_DefaultOpen)) return;

        if (ImGui::Button("Set for all sprites"))
        {
            for (Sprite& anotherSprite : spriteAtlas.Sprites)
            {
                anotherSprite.Origin = sprite.Origin;
            }
        }

        RenderOriginControl(sprite, originPlacement);
    }

    void GridControls(bool& isGridVisible, ImVec2i& cellSize, bool& isUniformCellSizeControl)
    {
        if (!ImGui::CollapsingHeader("Grid", ImGuiTreeNodeFlags_DefaultOpen)) return;

        ImGui::Checkbox("Show", &isGridVisible);
        ImGui::CellSizeControl(cellSize, isUniformCellSizeControl);
    }

    void SelectedSpriteWindow::RenderControls()
    {
        SpriteAtlas& spriteAtlas = _controller.GetSpriteAtlas();

        auto emptySprite = Sprite();

        Sprite& sprite = _controller.SelectedSpriteId
                        ? spriteAtlas.Sprites[_controller.SelectedSpriteId.value()]
                        : emptySprite;

        if (!_isEditingName)
            strncpy_s(_nameBuffer, sprite.Name.c_str(), sizeof _nameBuffer);

        ImGui::InputText("Name", _nameBuffer, sizeof _nameBuffer);

        if (ImGui::IsItemActivated())
            _isEditingName = true;

        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            _controller.RenameSelectedSprite(_nameBuffer);

            _isEditingName = false;
        }

        Texture2D* texture = _controller.GetTexture();

        SpriteRegionControls(sprite, texture);
        OriginControls(sprite, spriteAtlas, _originPlacement);
        GridControls(_isGridVisible,_cellSize, _isUniformCellSizeControl);
    }

    void SelectedSpriteWindow::Render()
    {
        if (!ImGui::Begin("Selection"))
        {
            ImGui::End();
            return;
        }

        RenderControls();
        RenderSelectionImage(_controller, _cellSize, _isGridVisible);

        ImGui::End();
    }
}