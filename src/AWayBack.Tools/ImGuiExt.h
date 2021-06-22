#pragma once

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

    struct ImVec2i
    {
        ImVec2i() { X = Y = 0; }
        ImVec2i(int16_t x, int16_t y)
            : X(x), Y(y)
        {
            
        }

        int16_t X, Y;
    };

    static void CheckerBoard(ImVec2 position, ImVec2i cellSize, ImVec2i gridSize)
    {
        const ImVec4 whiteCell = { 0.3f, 0.3f, 0.3f, 1.0f };
        const ImVec4 blackCell = { 0.4f, 0.4f, 0.4f, 1.0f };

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        auto isColorChange = false;

        auto gridMax = ImVec2(position.x + gridSize.X, position.y + gridSize.Y);

        for (int16_t y = 0; y < gridSize.Y; y += cellSize.Y)
        {
            for (int16_t x = 0; x < gridSize.X; x += cellSize.X)
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
}