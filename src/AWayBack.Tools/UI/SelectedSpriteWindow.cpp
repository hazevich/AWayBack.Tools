#include "SelectedSpriteWindow.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiExt.h"

namespace AWayBack
{
    SelectedSpriteWindow::SelectedSpriteWindow(SpriteEditorController& controller)
        : _controller(controller)
    {
        
    }

    void RenderSelectionImage(SpriteEditorController& controller)
    {
        ImGui::BeginChild("SelectionFrame", ImVec2(0, 0), true);

        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
        ImGui::ImVec2i cellSize = controller.GetCellSize();

        if (controller.SelectedSpriteId)
        {
            SpriteAtlas& spriteAtlas = controller.GetSpriteAtlas();
            Sprite& sprite = spriteAtlas.Sprites[controller.SelectedSpriteId.value()];
            Vector2 spriteSize = sprite.Max - sprite.Min;
            auto spritePosition = ImVec2((contentRegionAvail.x - spriteSize.X) * 0.5f,
                                         (contentRegionAvail.y - spriteSize.Y) * 0.5f);
            ImGui::SetCursorPos(spritePosition);

            ImGui::CheckerBoard(cellSize, ImGui::ImVec2i(spriteSize.X, spriteSize.Y));

            Texture2D* texture = controller.GetTexture();

            auto spriteuv0 = ImVec2(sprite.Min.X / texture->GetWidth(), sprite.Min.Y / texture->GetHeight());
            auto spriteuv1 = ImVec2(sprite.Max.X / texture->GetWidth(), sprite.Max.Y / texture->GetHeight());
            ImVec2 imageScreenPos = ImGui::GetCursorScreenPos();
            ImGui::Image(*texture, ImVec2(spriteSize.X, spriteSize.Y), spriteuv0, spriteuv1);

            const float borderOffset = 5;
            ImGui::Border(ImVec2(imageScreenPos.x - borderOffset, imageScreenPos.y - borderOffset),
                          ImVec2(spriteSize.X + borderOffset * 2, spriteSize.Y + borderOffset * 2),
                          ImGui::GetColorU32(ImVec4(1, 1, 1, 1)));
        }
        else
        {
            ImGui::CheckerBoard(cellSize, ImGui::ImVec2i(contentRegionAvail.x, contentRegionAvail.y));
        }

        ImGui::EndChild();
    }
    
    void RenderControls(SpriteEditorController& controller)
    {
        const int32_t spriteNameMaxSize = 1024;

        SpriteAtlas& spriteAtlas = controller.GetSpriteAtlas();

        auto emptySprite = Sprite();

        Sprite& sprite = controller.SelectedSpriteId
                        ? spriteAtlas.Sprites[controller.SelectedSpriteId.value()]
                        : emptySprite;

        char nameBuffer[spriteNameMaxSize] = { 0 };
        strncpy_s(nameBuffer, sprite.Name.c_str(), sizeof nameBuffer);

        if (ImGui::InputText("Name", nameBuffer, sizeof nameBuffer))
        {
            sprite.Name = std::string(nameBuffer);
        }

        Texture2D* texture = controller.GetTexture();

        Vector2 spriteSize = sprite.Max - sprite.Min;

        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

        ImGui::DragFloat("X", &sprite.Min.X, 1, 0, texture ? texture->GetWidth() - spriteSize.X : 0);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::DragFloat("Y", &sprite.Min.Y, 1, 0, texture ? texture->GetHeight() - spriteSize.Y : 0);
        ImGui::PopItemWidth();

        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
        
        if (ImGui::DragFloat("W", &spriteSize.X, 1, 1, texture ? texture->GetWidth() - sprite.Min.X : 0))
        {
            sprite.Max.X = sprite.Min.X + spriteSize.X;
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        if (ImGui::DragFloat("H", &spriteSize.Y, 1, 1, texture ? texture->GetHeight() - sprite.Min.Y : 0))
        {
            sprite.Max.Y = sprite.Min.Y + spriteSize.Y;
        }
        ImGui::PopItemWidth();

         ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

        ImGui::DragFloat("OX", &sprite.Min.X, 1, 0, spriteSize.X);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::DragFloat("OY", &sprite.Min.Y, 1, 0, spriteSize.Y);;
        ImGui::PopItemWidth();
    }

    void SelectedSpriteWindow::Render()
    {
        if (!ImGui::Begin("Selection"))
        {
            ImGui::End();
            return;
        }

        RenderControls(_controller);
        RenderSelectionImage(_controller);

        ImGui::End();
    }
}