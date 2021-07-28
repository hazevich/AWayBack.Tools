#include "SpritesWindow.h"

namespace AWayBack
{
    void RenderListOfSprites(SpriteEditorController& controller)
    {
        const int32_t itemSize = 70;

        const SpriteAtlas& spriteAtlas = controller.GetSpriteAtlas();
        Texture2D* texture = controller.GetTexture();

        ImGui::BeginChild("ListOfSprites", ImVec2(0, 0), true);

        for (int32_t i = 0; i < spriteAtlas.Sprites.size(); i++)
        {
            ImGui::PushID(i);

            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 84, 70, 255));
            if (ImGui::Button("X", ImVec2(20, itemSize)))
            {
                controller.RemoveSprite(i);

                i--;

                ImGui::PopID();
                ImGui::PopStyleColor();

                continue;
            }

            ImGui::PopStyleColor();

            ImGui::SameLine();

            if (i == controller.SelectedSpriteId)
            {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(46, 111, 230, 255));
            }

            ImGui::BeginChild("Tile", ImVec2(ImGui::GetContentRegionAvailWidth(), itemSize), false,
                              ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            const Sprite& sprite = spriteAtlas.Sprites[i];

            ImGui::BeginChild("Thumbnail", ImVec2(itemSize, itemSize));

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

            if (i == controller.SelectedSpriteId)
            {
                ImGui::PopStyleColor();
            }

            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
            {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    controller.SelectedSpriteId = i;
            }

            ImGui::EndChild();

            ImGui::PopID();            
        }

        ImGui::EndChild();
    }

    bool ConfirmClearPopup()
    {
        auto result = false;
        
        if (ImGui::BeginCenteredModal("Clear sprites", nullptr, ImGuiWindowFlags_NoResize))
        {
            ImGui::Text("Are you sure?\nThis will remove all the sprites from the sprite atlas.");

            ImGui::NewLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();

                result = false;
            }

            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 219, 111, 255));

            if (ImGui::Button("OK"))
            {
                ImGui::CloseCurrentPopup();

                result = true;
            }

            ImGui::PopStyleColor();

            ImGui::EndPopup();
        }

        return result;
    }

    void SpritesWindow::Render()
    {
        if (!ImGui::Begin("Sprites"))
        {
            ImGui::End();
            return;
        }

        if (ImGui::Button("Clear"))
        {
            ImGui::OpenPopup("Clear sprites");
        }

        RenderListOfSprites(_controller);

        if (ConfirmClearPopup())
        {
            _controller.ClearSprites();
        }

        ImGui::End();
    }
}
