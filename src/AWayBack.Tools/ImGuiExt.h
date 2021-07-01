#pragma once

#include "imgui.h"
#include "AWayBack/Graphics/Texture.h"
#include "SpriteAtlas.h"
#include "AWayBack/Utils/FileDialog.h"

#include "filesystem"

namespace ImGui
{
    static bool BeginCenteredModal(const char* name, bool* isOpen, ImGuiWindowFlags flags = ImGuiBackendFlags_None)
    {
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(.5f, .5f));

        return ImGui::BeginPopupModal(name, isOpen, flags);
    }

    static void Image(AWayBack::Texture2D& texture)
    {
        ImGui::Image((void*) (intptr_t) texture.GetTextureId(), ImVec2(texture.GetWidth(), texture.GetHeight()));
    }

    static void Image(AWayBack::Texture2D& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1)
    {
        ImGui::Image((void*) (intptr_t) texture.GetTextureId(), size, uv0, uv1);
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

    static void CheckerBoard(const ImVec2i& cellSize, const ImVec2i& gridSize)
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

    static void Grid(const ImVec2& position, const ImVec2i& cellSize, const ImVec2i& gridSize)
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

    static bool NewSpriteAtlas(AWayBack::SpriteAtlas& spriteAtlas, bool& isOpen)
    {
        namespace fs = std::filesystem;

        if (!ImGui::BeginCenteredModal("New spritesheet", &isOpen))
        {
            
            return false;
        }

        char stringBuffer[_MAX_PATH] = { 0 };
        strncpy_s(stringBuffer, spriteAtlas.Name.c_str(), sizeof stringBuffer);
        if (ImGui::InputText("Name", stringBuffer, sizeof stringBuffer))
        {
            spriteAtlas.Name = std::string(stringBuffer);
        }

        memset(stringBuffer, 0, sizeof stringBuffer);
        strncpy_s(stringBuffer, spriteAtlas.Path.c_str(), sizeof stringBuffer);
        if (ImGui::InputText("Folder", stringBuffer, sizeof stringBuffer))
        {
            spriteAtlas.Path = std::string(stringBuffer);
        }

        if (ImGui::Button("..."))
        {
            std::optional<std::string> folderPath = AWayBack::FileDialog::OpenFolder();

            if (folderPath)
            {
                spriteAtlas.Path = folderPath.value();
            }
        }

        ImGui::NewLine();

        memset(stringBuffer, 0, sizeof stringBuffer);
        strncpy_s(stringBuffer, spriteAtlas.TextureName.c_str(), sizeof stringBuffer);
        if (ImGui::InputText("Texture", stringBuffer, sizeof stringBuffer))
        {
            spriteAtlas.TextureName = std::string(stringBuffer);
        }

        ImGui::PushID("BrowseTexture");

        if (ImGui::Button("..."))
        {
            std::optional<std::string> texturePath = AWayBack::FileDialog::OpenFile("Textures (*.png)\0*.png\0");

            if (texturePath)
            {
                spriteAtlas.TextureName = texturePath.value();
            }
        }

        ImGui::PopID();

        if (ImGui::Button("OK"))
        {
            auto textureName = fs::path(spriteAtlas.TextureName).filename();
            fs::path destinationTexturePath = spriteAtlas.Path / textureName;

            if (destinationTexturePath != spriteAtlas.TextureName)
            {
                try 
                {
                    fs::copy(spriteAtlas.TextureName, destinationTexturePath.string(), fs::copy_options::overwrite_existing);
                }
                catch (const std::filesystem::filesystem_error& e)
                {
                    printf(e.what());
                    printf("\n");
                    ImGui::EndPopup();
                    return false;
                }
            }

            spriteAtlas.TextureName = textureName.string();
            spriteAtlas.Name += ".atlas";
            
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();

            return true;
        }

        ImGui::EndPopup();

        return false;
    }
}