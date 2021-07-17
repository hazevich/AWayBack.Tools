#include "SelectedSpriteWindow.h"
#include "imgui.h"
#include "ImGuiExt.h"

namespace AWayBack
{
    SelectedSpriteWindow::SelectedSpriteWindow(SpriteEditorController& controller)
        : _controller(controller)
    {
        
    }

    void SelectedSpriteWindow::Render()
    {
        if (!ImGui::Begin("Selection"))
        {
            ImGui::End();
            return;
        }

        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
        ImGui::ImVec2i cellSize = _controller.GetCellSize();

        if (_controller.SelectedSpriteId)
        {
            SpriteAtlas& spriteAtlas = _controller.GetSpriteAtlas();
            Sprite& sprite = spriteAtlas.Sprites[_controller.SelectedSpriteId.value()];
            Vector2 spriteSize = sprite.Max - sprite.Min;
            auto spritePosition = ImVec2((contentRegionAvail.x - spriteSize.X) * 0.5f,
                                         (contentRegionAvail.y - spriteSize.Y) * 0.5f);
            ImGui::SetCursorPos(spritePosition);

            ImGui::CheckerBoard(cellSize, ImGui::ImVec2i(spriteSize.X, spriteSize.Y));

            Texture2D* texture = _controller.GetTexture();

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

        ImGui::End();
    }

}