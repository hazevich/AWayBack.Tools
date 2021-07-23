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

    void RenderOriginDot(ImVec2 cursorPosition)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        drawList->AddCircleFilled(cursorPosition, 3, IM_COL32(51, 250, 137, 255));
    }

    void RenderSelectionImage(SpriteEditorController& controller)
    {
        ImGui::BeginChild("SelectionFrame", ImVec2(0, 0), true);

        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
        ImVec2i cellSize = controller.GetCellSize();

        if (controller.SelectedSpriteId)
        {
            SpriteAtlas& spriteAtlas = controller.GetSpriteAtlas();
            Sprite& sprite = spriteAtlas.Sprites[controller.SelectedSpriteId.value()];
            Vector2 spriteSize = sprite.Max - sprite.Min;
            auto spritePosition = ImVec2((contentRegionAvail.x - spriteSize.X) * 0.5f,
                                         (contentRegionAvail.y - spriteSize.Y) * 0.5f);
            ImGui::SetCursorPos(spritePosition);

            ImGui::CheckerBoard(cellSize, ImVec2i(spriteSize.X, spriteSize.Y));

            Texture2D* texture = controller.GetTexture();

            auto spriteuv0 = ImVec2(sprite.Min.X / texture->GetWidth(), sprite.Min.Y / texture->GetHeight());
            auto spriteuv1 = ImVec2(sprite.Max.X / texture->GetWidth(), sprite.Max.Y / texture->GetHeight());
            ImVec2 imageScreenPos = ImGui::GetCursorScreenPos();
            ImGui::Image(*texture, ImVec2(spriteSize.X, spriteSize.Y), spriteuv0, spriteuv1);

            const float borderOffset = 5;
            ImGui::Border(ImVec2(imageScreenPos.x - borderOffset, imageScreenPos.y - borderOffset),
                          ImVec2(spriteSize.X + borderOffset * 2, spriteSize.Y + borderOffset * 2),
                          ImGui::GetColorU32(ImVec4(1, 1, 1, 1)));

            RenderOriginDot(ImVec2(imageScreenPos.x + sprite.Origin.X, imageScreenPos.y + sprite.Origin.Y));
        }
        else
        {
            ImGui::CheckerBoard(cellSize, ImVec2i(contentRegionAvail.x, contentRegionAvail.y));
        }

        ImGui::EndChild();
    }

    bool Vector2Control(const char* label, Vector2& vec2, Vector2 min, Vector2 max)
    {
        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

        ImGuiStyle& style = ImGui::GetStyle();

        ImGui::PushID(label);

        bool xResult = ImGui::DragFloat("##X", &vec2.X, 1, min.X, max.X);
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        bool yResult = ImGui::DragFloat("##Y", &vec2.Y, 1, min.Y, max.Y);
        ImGui::PopItemWidth();
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        ImGui::PopID();

        ImGui::Text(label);

        return xResult || yResult;
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

        if (Vector2Control(
            "Sprite region position", 
            sprite.Min, 
            Vector2(0, 0), 
            texture ? Vector2(texture->GetWidth() - spriteSize.X, texture->GetHeight() - spriteSize.Y) : Vector2(0, 0)
        ))
        {
            sprite.Max = sprite.Min + spriteSize;
        }

        if (Vector2Control(
            "Sprite region size", 
            spriteSize, 
            Vector2(1, 1), 
            texture ? Vector2(texture->GetWidth() - sprite.Min.X, texture->GetHeight() - sprite.Min.Y) : Vector2(0, 0)
        ))
        {
            sprite.Max = sprite.Min + spriteSize;
        }

        Vector2Control("Origin", sprite.Origin, Vector2(0, 0), spriteSize);

        ImGui::SameLine();

        if (ImGui::Button("Set for all sprites"))
        {
            for (Sprite& anotherSprite : spriteAtlas.Sprites)
            {
                anotherSprite.Origin = sprite.Origin;
            }
        }
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