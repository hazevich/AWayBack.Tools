#include "CanvasWindow.h"

#include "algorithm"
#include "imgui.h"
#include "ImGuiExt.h"

namespace AWayBack
{
    void DrawSelectedCellSequenceNumber(ImDrawList* drawList, ImVec2 position, ImVec2i cellSize, int32_t sequenceNumber)
    {
        const float backgroundCircleRadius = 16;

        std::string sequenceNumberString = std::to_string(sequenceNumber);

        ImVec2 centerPosition = ImVec2(position.x + cellSize.X * 0.5f, position.y + cellSize.Y * 0.5f);
        ImVec2 textSize = ImGui::CalcTextSize(sequenceNumberString.c_str());

        drawList->AddCircleFilled(centerPosition, backgroundCircleRadius, IM_COL32(0, 0, 0, 75.0f));

        ImVec2 textPosition = ImVec2(centerPosition.x - textSize.x * 0.5f, centerPosition.y - textSize.y * 0.5f);
        ImGui::SetCursorScreenPos(textPosition);
        ImGui::Text(sequenceNumberString.c_str());
    }

    void RenderSelectedCell(
        ImDrawList* drawList, 
        int32_t cellIndex, 
        SpriteEditorController& controller,
        ImVec2 cursorPosition,
        int32_t sequenceNumber
    )
    {
        ImVec2i cellSize = controller.GetCellSize();
        int32_t gridWidth = controller.GridWidth;
        ImVec2 relativePosition = GetPositonFromCell(cellIndex, cellSize, gridWidth);
        float x = relativePosition.x + cursorPosition.x;
        float y = relativePosition.y + cursorPosition.y;
        auto position = ImVec2(x, y);

        drawList->AddRect(position, ImVec2(x + cellSize.X, y + cellSize.Y),
                          ImGui::GetColorU32({1.0f, 0.0f, 0.0f, 1.0f}));

        DrawSelectedCellSequenceNumber(drawList, position, cellSize, sequenceNumber);
    }

    void RenderGridSequence(SpriteEditorController& controller, ImVec2 cursorScreenPos)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (int32_t i = controller.SliceStart; i < controller.SliceEnd; i++)
        {
            RenderSelectedCell(drawList, i, controller, cursorScreenPos, i + 1);
        }
    }

    void RenderGridSelection(SpriteEditorController& controller, ImVec2 cursorScreenPos)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        int32_t sequenceNumber = 1;
        for (int32_t& cell : controller.SelectedCells)
        {
            RenderSelectedCell(drawList, cell, controller, cursorScreenPos, sequenceNumber);
            sequenceNumber++;
        }
    }

    void RenderFreehand(SpriteEditorController& controller, ImVec2 cursorScreenPos)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        drawList->AddRect(
            ImVec2(controller.SelectedRegion.Min.x + cursorScreenPos.x,
                   controller.SelectedRegion.Min.y + cursorScreenPos.y),
            ImVec2(controller.SelectedRegion.Max.x + cursorScreenPos.x,
                   controller.SelectedRegion.Max.y + cursorScreenPos.y),
            ImGui::GetColorU32({1.0f, 0.0f, 0.0f, 1.0f})
        );
    }

    void RenderSelectedRegions(SpriteEditorController& controller, ImVec2 cursorScreenPos)
    {
        switch (controller.SlicingType)
        {
        case SlicingType::GridSequence:
            {
                RenderGridSequence(controller, cursorScreenPos);
                break;
            }
        case SlicingType::GridSelection:
            {
                RenderGridSelection(controller, cursorScreenPos);
                break;
            }
        case SlicingType::Freehand:
            {
                RenderFreehand(controller, cursorScreenPos);
                break;
            }
        }
    }

    void RenderSelectedSpriteRegion(SpriteEditorController& controller, ImVec2 cursorScreenPos)
    {
        if (controller.SelectedSpriteId)
        {
            const Sprite& sprite = controller.GetSprite(controller.SelectedSpriteId.value());

            ImDrawList* drawList = ImGui::GetWindowDrawList();

            drawList->AddRectFilled(
                ImVec2(cursorScreenPos.x + sprite.Min.X, cursorScreenPos.y + sprite.Min.Y),
                ImVec2(cursorScreenPos.x + sprite.Max.X, cursorScreenPos.y + sprite.Max.Y),
                ImGui::GetColorU32({0.0f, 0.0f, 1.0f, 0.3f})
            );
        }
    }

    void SetCursorPos(Texture2D& texture)
    {
        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
        ImVec2 cursorPos = ImGui::GetCursorPos();
        auto textureWidth = (float)texture.GetWidth();
        auto textureHeight = (float)texture.GetHeight();

        auto newCursorPos = ImVec2(
            contentRegionAvail.x > textureWidth
                ? (contentRegionAvail.x - textureWidth) * 0.5f
                : cursorPos.x,
            contentRegionAvail.y > textureHeight
                ? (contentRegionAvail.y - textureHeight) * 0.5f
                : cursorPos.y
        );

        ImGui::SetCursorPos(newCursorPos);
    }

    void OnMouseLeftButtonClicked(SpriteEditorController& controller, ImVec2 mousePosition)
    {
        if (controller.SlicingType != SlicingType::GridSelection) return;

        ImVec2i cellSize = controller.GetCellSize();
        int32_t cell = GetCellFromPosition(mousePosition, cellSize, controller.GridWidth);

        if (std::find(controller.SelectedCells.begin(), controller.SelectedCells.end(), cell) == controller.
            SelectedCells.end())
        {
            controller.SelectedCells.push_back(cell);
        }
    }

    void OnMouseRightButtonClicked(SpriteEditorController& controller, ImVec2 mousePosition)
    {
        if (controller.SlicingType != SlicingType::GridSelection) return;

        ImVec2i cellSize = controller.GetCellSize();
        int32_t cell = GetCellFromPosition(mousePosition, cellSize, controller.GridWidth);

        auto iterator = std::find(controller.SelectedCells.begin(), controller.SelectedCells.end(), cell);

        if (iterator != controller.SelectedCells.end())
        {
            controller.SelectedCells.erase(iterator);
        }
    }

    void OnMouseDragging(SpriteEditorController& controller, ImVec2 startPosition, ImVec2 endPosition)
    {
        if (controller.SlicingType != SlicingType::Freehand) return;

        float minx = std::min(startPosition.x, endPosition.x);
        float miny = std::min(startPosition.y, endPosition.y);

        float maxx = std::max(startPosition.x, endPosition.x);
        float maxy = std::max(startPosition.y, endPosition.y);

        controller.SelectedRegion = {ImVec2(minx, miny), ImVec2(maxx, maxy)};
    }
    
    void CanvasWindow::Render()
    {
        if (!ImGui::Begin("Canvas", 0, ImGuiWindowFlags_HorizontalScrollbar))
        {
            ImGui::End();
            return;
        }

        Texture2D* texture = _controller.GetTexture();

        if (texture)
        {
            SetCursorPos(*texture);
            ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();

            ImVec2i cellSize = _controller.GetCellSize();
            ImVec2i textureSize = ImVec2i(texture->GetWidth(), texture->GetHeight());

            ImGui::CheckerBoard(cellSize, textureSize);

            ImGui::Image(*texture);

            ImVec2 mousePosition = ImGui::GetMousePos() - cursorScreenPos;

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                OnMouseLeftButtonClicked(_controller, mousePosition);
            }

            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                OnMouseRightButtonClicked(_controller, mousePosition);
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            {
                ImGuiIO& io = ImGui::GetIO();
                ImVec2 startPosition = io.MouseClickedPos[ImGuiMouseButton_Left] - cursorScreenPos;
                OnMouseDragging(_controller, startPosition, mousePosition);
            }


            if (_controller.SlicingType == SlicingType::GridSelection || _controller.SlicingType == SlicingType::GridSequence)
                ImGui::Grid(cursorScreenPos, cellSize, textureSize);

            RenderSelectedRegions(_controller, cursorScreenPos);
            RenderSelectedSpriteRegion(_controller, cursorScreenPos);
        }

        ImGui::End();
    }
}
