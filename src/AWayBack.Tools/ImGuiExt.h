#pragma once

#include <optional>

#include "imgui.h"
#include "AWayBack/Graphics/Texture.h"

inline ImVec2 operator-(const ImVec2& left, const ImVec2& right)
{
    return ImVec2(left.x - right.x, left.y - right.y);
}

union ImVec2i
{
    ImVec2i() = default;

    ImVec2i(int32_t x, int32_t y)
        : X(x), Y(y)
    {
    
    }

    struct
    {
        int32_t X, Y;
    };

    int32_t Components[2];
};

namespace ImGui
{
    bool BeginCenteredModal(const char* name, bool* isOpen, ImGuiWindowFlags flags = ImGuiBackendFlags_None);

    void Image(AWayBack::Texture2D& texture);

    void Image(AWayBack::Texture2D& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1);
    
    void CheckerBoard(const ImVec2i& cellSize, const ImVec2i& size);    

    void Grid(const ImVec2& position, const ImVec2i& cellSize, const ImVec2i& size);
    
    void Border(const ImVec2& cursorScreenPos, const ImVec2& size, ImU32 color);

    bool CellSizeControl(ImVec2i& cellSize, bool& isUniformCellSizeControl);

    bool ButtonEx(const char* label, const ImVec2& size = ImVec2(0, 0), std::optional<bool> isDisabled = std::nullopt);
}