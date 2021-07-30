#include "ImGuiExt.h"

#include "imgui_internal.h"

namespace ImGui
{
    bool BeginCenteredModal(const char* name, bool* isOpen, ImGuiWindowFlags flags)
    {
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(.5f, .5f));

        return ImGui::BeginPopupModal(name, isOpen, flags);
    }

    void Image(AWayBack::Texture2D& texture)
    {
        ImGui::Image((void*) (intptr_t) texture.GetTextureId(), ImVec2(texture.GetWidth(), texture.GetHeight()));
    }

    void Image(AWayBack::Texture2D& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1)
    {
        ImGui::Image((void*) (intptr_t) texture.GetTextureId(), size, uv0, uv1);
    }

    void CheckerBoard(const ImVec2i& cellSize, const ImVec2i& size)
    {
        const ImVec4 whiteCell = { 0.3f, 0.3f, 0.3f, 1.0f };
        const ImVec4 blackCell = { 0.4f, 0.4f, 0.4f, 1.0f };

        ImVec2 position = ImGui::GetCursorScreenPos();

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        auto isColorChange = false;
        auto isLineChange = false;

        auto gridMax = ImVec2(position.x + size.X, position.y + size.Y);

        for (int32_t y = 0; y < size.Y; y += cellSize.Y)
        {
            for (int32_t x = 0; x < size.X; x += cellSize.X)
            {
                auto cellMin = ImVec2(x + position.x, y + position.y);
                auto cellMax = ImVec2(cellMin.x + cellSize.X, cellMin.y + cellSize.Y);

                if (cellMax.x > gridMax.x)
                    cellMax.x = gridMax.x;

                if (cellMax.y > gridMax.y)
                    cellMax.y = gridMax.y;

                drawList->AddRectFilled(cellMin, cellMax, ImGui::GetColorU32(isColorChange ? blackCell : whiteCell));
                isColorChange = !isColorChange;
            }

            isLineChange = !isLineChange;
            isColorChange = isLineChange;
        }
    }

    void Grid(const ImVec2& position, const ImVec2i& cellSize, const ImVec2i& size)
    {
        const ImVec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        auto gridMax = ImVec2(position.x + size.X, position.y + size.Y);

        for (int32_t y = 0; y < size.Y; y += cellSize.Y)
        {
            ImVec2 start = ImVec2(position.x, position.y + y);
            ImVec2 end = ImVec2(gridMax.x, position.y + y);

            drawList->AddLine(start, end, ImGui::GetColorU32(color));
        }

        for (int32_t x = 0; x < size.X; x += cellSize.X)
        {
            ImVec2 start = ImVec2(position.x + x, position.y);
            ImVec2 end = ImVec2(position.x + x, gridMax.y);

            drawList->AddLine(start, end, ImGui::GetColorU32(color));
        }
    }

    void Border(const ImVec2& cursorScreenPos, const ImVec2& size, ImU32 color)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRect(cursorScreenPos, ImVec2(cursorScreenPos.x + size.x, cursorScreenPos.y + size.y), color);
    }

    bool CellSizeControl(ImVec2i& cellSize, bool& isUniformCellSizeControl)
    {
        if (isUniformCellSizeControl)
        {
            if (ImGui::DragInt("##Cell size", &cellSize.X, 1, 1, INT32_MAX))
            {
                cellSize.Y = cellSize.X;

                return true;
            }
        }
        else if (ImGui::DragInt2("##Cell size", cellSize.Components, 1, 1, INT32_MAX))
        {
            return true;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cell size"))
        {
            isUniformCellSizeControl = !isUniformCellSizeControl;
        }

        return false;
    }

    bool ButtonEx(const char* label, const ImVec2& size, std::optional<bool> isEnabled)
    {
        if (isEnabled && !isEnabled.value())
            ImGui::PushDisabled();

        bool result = ImGui::Button(label, size);
        
        if (isEnabled && !isEnabled.value())
            ImGui::PopDisabled();

        return result;
    }

}
