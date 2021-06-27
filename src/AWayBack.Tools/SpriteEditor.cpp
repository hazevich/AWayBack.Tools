#include "SpriteEditor.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiExt.h"
#include "algorithm"
#include "SpriteAtlasSerializer.h"
#include "fstream"

namespace AWayBack
{
    void SpriteEditor::SetTexture(Texture2D* texture)
    {
        _texture = texture;
        _spriteAtlas = {_texture->GetName(), _texture->GetName(), std::vector<Sprite>()};
        _gridWidth = _texture->GetWidth() / _cellWidth;
        _gridHeight = _texture->GetHeight() / _cellHeight;
    }

    void SpriteEditor::Render()
    {
        RenderCanvas();
        RenderControls();
        RenderSprites();
    }

    void SpriteEditor::RenderCanvas()
    {
        if (!ImGui::Begin("Canvas")) return;

        ImGui::BeginChild("CanvasFrame",
                          ImVec2(0, ImGui::GetContentRegionAvail().y),
                          true,
                          ImGuiWindowFlags_HorizontalScrollbar
                          | ImGuiWindowFlags_AlwaysAutoResize
        );

        if (_texture)
        {
            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
            ImVec2 cursorPos = ImGui::GetCursorPos();
            auto textureWidth = (float)_texture->GetWidth();
            auto textureHeight = (float)_texture->GetHeight();

            auto newCursorPos = ImVec2(
                contentRegionAvail.x > textureHeight
                    ? (contentRegionAvail.x - textureWidth) * 0.5f
                    : cursorPos.x,
                contentRegionAvail.y > textureWidth
                    ? (contentRegionAvail.y - textureHeight) * 0.5f
                    : cursorPos.x
            );

            ImGui::SetCursorPos(newCursorPos);

            ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();

            auto cellSize = ImGui::ImVec2i(_cellWidth, _cellHeight),
                 gridSize = ImGui::ImVec2i(_texture->GetWidth(), _texture->GetHeight());

            ImGui::CheckerBoard(cellSize, gridSize);

            ImGui::Image(*_texture);

            ImGui::Grid(cursorScreenPos, cellSize, gridSize);

            ImDrawList* drawList = ImGui::GetWindowDrawList();

            for (int32_t i = _sliceStart; i < _sliceEnd; i++)
            {
                int32_t x = i % _gridWidth * _cellWidth + cursorScreenPos.x;
                int32_t y = i / _gridWidth * _cellHeight + cursorScreenPos.y;

                drawList->AddRect(ImVec2(x, y), ImVec2(x + _cellWidth, y + _cellHeight), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)));
            }
        }

        ImGui::EndChild();

        ImGui::End();
    }

    void SpriteEditor::RenderControls()
    {
        if (!ImGui::Begin("Controls")) return;

        RenderSpriteAtlasHeader();
        RenderSlicingControls();

        ImGui::End();
    }

    void SpriteEditor::RenderSpriteAtlasHeader()
    {
        if (!ImGui::CollapsingHeader("Sprite atlas", ImGuiTreeNodeFlags_DefaultOpen)) return;

        ImGui::LabelText("Sprite Atlas Name", _spriteAtlas.Name.c_str());
        ImGui::NewLine();
        ImGui::LabelText("Texture name", _spriteAtlas.TextureName.c_str());

        if (ImGui::Button("Save"))
        {
            std::ofstream file;
            file.open(_spriteAtlas.Name + ".atlas");
            SpriteAtlasSerializer::SerializeToFile(file, _spriteAtlas);
            file.close();
        }
    }

    void SpriteEditor::RenderSlicingControls()
    {
        if (!ImGui::CollapsingHeader("Slicing", ImGuiTreeNodeFlags_DefaultOpen)) return;

        RenderGridSequenceSlicingControls();

        if (ImGui::Button("Slice"))
        {
            for (int32_t i = _sliceStart; i < _sliceEnd; i++)
            {
                int32_t x = i % _gridWidth * _cellWidth;
                int32_t y = i / _gridWidth * _cellHeight;

                auto min = Vector2(x, y);
                auto max = Vector2(x + _cellWidth, y + _cellHeight);
                auto name = _spriteAtlas.Name + std::to_string(_spriteAtlas.Sprites.size());
                Sprite sprite = { name, min, max, Vector2() };
                _spriteAtlas.Sprites.push_back(sprite);
            }
        }
    }

    void SpriteEditor::RenderGridSequenceSlicingControls()
    {
        ImGui::DragInt("Slice start", &_sliceStart, 1, 0, _sliceEnd - 1, "%d",
                       _sliceEnd == 1 ? ImGuiSliderFlags_ReadOnly : ImGuiSliderFlags_None);
        ImGui::DragInt("Slice end", &_sliceEnd, 1, _sliceStart + 1, _gridWidth * _gridHeight);
    }

    void SpriteEditor::RenderSprites()
    {
        const int32_t itemSize = 70;

        if (!ImGui::Begin("Sprites")) return;

        char childTitleBuffer[300] = {0};
        
        for (Sprite& sprite : _spriteAtlas.Sprites)
        {

            snprintf(childTitleBuffer, sizeof childTitleBuffer, "Tile %s", sprite.Name.c_str());
            ImGui::BeginChild(childTitleBuffer, ImVec2(ImGui::GetContentRegionAvailWidth(), itemSize), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            
            snprintf(childTitleBuffer, sizeof childTitleBuffer, "Thumbnail %s", sprite.Name.c_str());
            ImGui::BeginChild(childTitleBuffer, ImVec2(itemSize, itemSize));

            float width = sprite.Max.X - sprite.Min.X;
            float height = sprite.Max.Y - sprite.Min.Y;

            float scaleX = itemSize / width;
            float scaleY = itemSize / height;
            float minScale = std::min(scaleX, scaleY);

            float thumbnailWidth = width * minScale;
            float thumbnailHeight = height * minScale;

            float textureWidth = _texture->GetWidth();
            float textureHeight = _texture->GetHeight();

            auto thumbnailuv0 = ImVec2(sprite.Min.X / textureWidth, sprite.Min.Y / textureHeight);
            auto thumbnailuv1 = ImVec2(sprite.Max.X / textureWidth, sprite.Max.Y / textureHeight);

            ImGui::Image(*_texture, ImVec2(thumbnailWidth, thumbnailHeight), thumbnailuv0, thumbnailuv1);

            ImGui::EndChild();

            ImGui::SameLine();

            ImVec2 textSize = ImGui::CalcTextSize(sprite.Name.c_str());

            ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - textSize.y) * 0.5f);
            
            ImGui::Text(sprite.Name.c_str());

            ImGui::EndChild();
        }

        ImGui::End();
    }

}
