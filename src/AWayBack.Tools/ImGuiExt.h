﻿#pragma once

#include "imgui.h"
#include "AWayBack/Graphics/Texture.h"

namespace ImGui
{
    static bool BeginCenteredModal(const char* name, ImGuiWindowFlags flags = ImGuiBackendFlags_None, bool isClosable = true)
    {
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(.5f, .5f));

        if (isClosable)
        {
            auto isOpen = true;

            return ImGui::BeginPopupModal(name, &isOpen, flags);
        }

        return ImGui::BeginPopupModal(name, nullptr, flags);
    }

    static void Image(AWayBack::Texture2D& texture)
    {
        ImGui::Image((void*) (intptr_t) texture.GetTextureId(), ImVec2(texture.GetWidth(), texture.GetHeight()));
    }

    static void Image(AWayBack::Texture2D& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1)
    {
        ImGui::Image((void*) (intptr_t) texture.GetTextureId(), size, uv0, uv1);;
    }

    struct ImVec2i
    {
        ImVec2i() { X = Y = 0; }
        ImVec2i(int32_t x, int32_t y)
            : X(x), Y(y)
        {
            
        }

        int32_t X, Y;
    };

    static void CheckerBoard(ImVec2i cellSize, ImVec2i gridSize)
    {
        const ImVec4 whiteCell = { 0.3f, 0.3f, 0.3f, 1.0f };
        const ImVec4 blackCell = { 0.4f, 0.4f, 0.4f, 1.0f };

        ImVec2 position = ImGui::GetCursorScreenPos();

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        auto isColorChange = false;

        auto gridMax = ImVec2(position.x + gridSize.X, position.y + gridSize.Y);

        for (int32_t y = 0; y < gridSize.Y; y += cellSize.Y)
        {
            for (int32_t x = 0; x < gridSize.X; x += cellSize.X)
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

            isColorChange = !isColorChange;
        }
    }

    static void Grid(ImVec2 position, ImVec2i cellSize, ImVec2i gridSize)
    {
        const ImVec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        auto gridMax = ImVec2(position.x + gridSize.X, position.y + gridSize.Y);

        for (int32_t y = 0; y < gridSize.Y; y += cellSize.Y)
        {
            ImVec2 start = ImVec2(position.x, position.y + y);
            ImVec2 end = ImVec2(gridMax.x, position.y + y);

            drawList->AddLine(start, end, ImGui::GetColorU32(color));
        }

        for (int32_t x = 0; x < gridSize.X; x += cellSize.X)
        {
            ImVec2 start = ImVec2(position.x + x, position.y);
            ImVec2 end = ImVec2(position.x + x, gridMax.y);

            drawList->AddLine(start, end, ImGui::GetColorU32(color));
        }
    }
}