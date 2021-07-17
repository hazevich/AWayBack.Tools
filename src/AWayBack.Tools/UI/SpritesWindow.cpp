#include "SpritesWindow.h"

namespace AWayBack
{
    void SpritesWindow::Render()
    {
        const int32_t itemSize = 70;

        if (!ImGui::Begin("Sprites"))
        {
            ImGui::End();
            return;
        }

        SpriteAtlas& spriteAtlas = _controller.GetSpriteAtlas();
        Texture2D* texture = _controller.GetTexture();

        char childTitleBuffer[300] = {0};

        for (int32_t i = 0; i < spriteAtlas.Sprites.size(); i++)
        {
            Sprite& sprite = spriteAtlas.Sprites[i];
            snprintf(childTitleBuffer, sizeof childTitleBuffer, "Tile %s", sprite.Name.c_str());

            if (i == _controller.SelectedSpriteId)
            {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(46, 111, 230, 255));
            }

            ImGui::BeginChild(childTitleBuffer, ImVec2(ImGui::GetContentRegionAvailWidth(), itemSize), false,
                              ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            snprintf(childTitleBuffer, sizeof childTitleBuffer, "Thumbnail %s", sprite.Name.c_str());
            ImGui::BeginChild(childTitleBuffer, ImVec2(itemSize, itemSize));

            float width = sprite.Max.X - sprite.Min.X;
            float height = sprite.Max.Y - sprite.Min.Y;

            float scaleX = itemSize / width;
            float scaleY = itemSize / height;
            float minScale = std::min(scaleX, scaleY);

            float thumbnailWidth = width * minScale;
            float thumbnailHeight = height * minScale;

            float textureWidth = texture->GetWidth();
            float textureHeight = texture->GetHeight();

            auto thumbnailuv0 = ImVec2(sprite.Min.X / textureWidth, sprite.Min.Y / textureHeight);
            auto thumbnailuv1 = ImVec2(sprite.Max.X / textureWidth, sprite.Max.Y / textureHeight);

            ImGui::Image(*texture, ImVec2(thumbnailWidth, thumbnailHeight), thumbnailuv0, thumbnailuv1);

            ImGui::EndChild();

            ImGui::SameLine();

            ImVec2 textSize = ImGui::CalcTextSize(sprite.Name.c_str());

            ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - textSize.y) * 0.5f);

            ImGui::Text(sprite.Name.c_str());

            if (i == _controller.SelectedSpriteId)
            {
                ImGui::PopStyleColor();
            }

            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
            {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    _controller.SelectedSpriteId = i;
            }

            ImGui::EndChild();

        }

        ImGui::End();
    }
}
