#include "SpriteEditor.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiExt.h"
#include "algorithm"
#include "SpriteAtlasSerializer.h"

namespace AWayBack
{
    SpriteEditor::~SpriteEditor()
    {
    }

    void SpriteEditor::LoadTexture(const std::string& texturePath)
    {
    }

    void SpriteEditor::CreateNewSpriteAtlas()
    {
        _isNewSpriteAtlasRequested = true;
    }

    void SpriteEditor::LoadSpriteAtlas(const std::string& spriteAtlasPath)
    {
        _controller.LoadSpriteAtlas(spriteAtlasPath);
    }

    void SpriteEditor::Render()
    {
        if (_isNewSpriteAtlasRequested)
        {
            ImGui::OpenPopup("New spritesheet");
            _isNewSpriteAtlasRequested = false;
            _spriteAtlasData = {};
        }

        RenderNewSpriteAtlasModal();

        RenderCanvas();
        RenderControls();
        RenderSprites();
        RenderSelectedSprite();
    }

    void SpriteEditor::RenderCanvas()
    {
        if (!ImGui::Begin("Canvas"))
        {
            ImGui::End();
            return;
        }

        ImGui::BeginChild("CanvasFrame",
                          ImVec2(0, ImGui::GetContentRegionAvail().y),
                          true,
                          ImGuiWindowFlags_HorizontalScrollbar
                          | ImGuiWindowFlags_AlwaysAutoResize
        );

        Texture2D* texture = _controller.GetTexture();

        if (texture)
        {
            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
            ImVec2 cursorPos = ImGui::GetCursorPos();
            auto textureWidth = (float)texture->GetWidth();
            auto textureHeight = (float)texture->GetHeight();

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

            ImGui::ImVec2i cellSize = _controller.GetCellSize();
            ImGui::ImVec2i textureSize = ImGui::ImVec2i(texture->GetWidth(), texture->GetHeight());

            ImGui::CheckerBoard(cellSize, textureSize);

            ImGui::Image(*texture);

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                ImVec2 mousePosition = ImGui::GetMousePos() - cursorScreenPos;
                int32_t cell = GetCellFromPosition(mousePosition, cellSize, _controller.GridWidth);

                if (std::find(_controller.SelectedCells.begin(), _controller.SelectedCells.end(), cell) == _controller.SelectedCells.end())
                {
                    _controller.SelectedCells.push_back(cell);
                }
            }
            else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                ImVec2 mousePosition = ImGui::GetMousePos() - cursorScreenPos;
                int32_t cell = GetCellFromPosition(mousePosition, cellSize, _controller.GridWidth);

                auto iterator = std::find(_controller.SelectedCells.begin(), _controller.SelectedCells.end(), cell);

                if (iterator != _controller.SelectedCells.end())
                {
                    _controller.SelectedCells.erase(iterator);
                }
            }

            ImGui::Grid(cursorScreenPos, cellSize, textureSize);

            RenderSelectedRegions(cursorScreenPos);
            RenderSelectedSpriteRegion(cursorScreenPos);
        }

        ImGui::EndChild();

        ImGui::End();
    }

    void SpriteEditor::RenderSelectedRegions(ImVec2 cursorScreenPos)
    {
        switch (_controller.SlicingType)
        {
            case SlicingType::GridSequence:
            {
                RenderGridSequence(cursorScreenPos);
                break;
            }
            case SlicingType::GridSelection:
            {
                RenderGridSelection(cursorScreenPos);
                break;
            }
        }
    }

    void SpriteEditor::RenderSelectedSpriteRegion(ImVec2 cursorScreenPos)
    {
        if (_controller.SelectedSpriteId)
        {
            int32_t value = _controller.SelectedSpriteId.value();
            SpriteAtlas& spriteAtlas = _controller.GetSpriteAtlas();
            if (spriteAtlas.Sprites.size() > value)
            {
                Sprite& sprite = spriteAtlas.Sprites[value];

                ImDrawList* drawList = ImGui::GetWindowDrawList();

                drawList->AddRectFilled(
                    ImVec2(cursorScreenPos.x + sprite.Min.X, cursorScreenPos.y + sprite.Min.Y),
                    ImVec2(cursorScreenPos.x + sprite.Max.X, cursorScreenPos.y + sprite.Max.Y),
                    ImGui::GetColorU32({0.0f, 0.0f, 1.0f, 0.3f})
                );
            }
        }
    }

    void RenderSelectedCell(ImDrawList* drawList, int32_t cellIndex, int32_t gridWidth, ImGui::ImVec2i cellSize, ImVec2 cursorPosition)
    {
        ImVec2 relativePosition = GetPositonFromCell(cellIndex, cellSize, gridWidth);
        float x = relativePosition.x + cursorPosition.x;
        float y = relativePosition.y + cursorPosition.y;

        drawList->AddRect(ImVec2(x, y), ImVec2(x + cellSize.X, y + cellSize.Y),ImGui::GetColorU32({1.0f, 0.0f, 0.0f, 1.0f}));
    }

    void SpriteEditor::RenderGridSequence(ImVec2 cursorScreenPos)
    {
        ImGui::ImVec2i cellSize = _controller.GetCellSize();
        
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (int32_t i = _controller.SliceStart; i < _controller.SliceEnd; i++)
        {
            RenderSelectedCell(drawList, i, _controller.GridWidth, cellSize, cursorScreenPos);
        }
    }

    void SpriteEditor::RenderGridSelection(ImVec2 cursosScreenPos)
    {
        ImGui::ImVec2i cellSize = _controller.GetCellSize();
        
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (int32_t &cell : _controller.SelectedCells)
        {
            RenderSelectedCell(drawList, cell, _controller.GridWidth, cellSize, cursosScreenPos);
        }
    }

    void SpriteEditor::RenderControls()
    {
        if (!ImGui::Begin("Controls"))
        {
            ImGui::End();
            return;
        }

        RenderSpriteAtlasHeader();
        RenderSlicingControls();

        ImGui::End();
    }

    void SpriteEditor::RenderSpriteAtlasHeader()
    {
        if (!ImGui::CollapsingHeader("Sprite atlas", ImGuiTreeNodeFlags_DefaultOpen)) return;

        SpriteAtlas& spriteAtlas = _controller.GetSpriteAtlas();

        ImGui::LabelText("Sprite Atlas Name", spriteAtlas.Name.c_str());
        ImGui::NewLine();
        ImGui::LabelText("Texture name", spriteAtlas.TextureName.c_str());

        if (ImGui::Button("Save"))
        {
            _controller.Save();
        }
    }

    void SpriteEditor::RenderSlicingControls()
    {
        if (!ImGui::CollapsingHeader("Slicing", ImGuiTreeNodeFlags_DefaultOpen)) return;

        auto slicingTypeInt = (int32_t) _controller.SlicingType;
        if (ImGui::Combo("Slicing type", &slicingTypeInt, "Grid sequence\0Grid selection\0Freehand"))
        {
            _controller.SlicingType = (SlicingType) slicingTypeInt;
        }

        ImGui::ImVec2i cellSize = _controller.GetCellSize();

        if (_isUniformCellSizeControl)
        {
            if (ImGui::DragInt("##Cell size", &cellSize.X, 1, 1, INT32_MAX))
            {
                cellSize.Y = cellSize.X;

                _controller.SetCellSize(cellSize);
            }
        }
        else if (ImGui::DragInt2("##Cell size", cellSize.Components, 1, 1, INT32_MAX))
        {
            _controller.SetCellSize(cellSize);
        }

        ImGui::SameLine();

        if (ImGui::Button("Cell size"))
        {
            _isUniformCellSizeControl = !_isUniformCellSizeControl;
        }

        if (_controller.SlicingType == SlicingType::GridSequence)
            RenderGridSequenceSlicingControls();

        if (ImGui::Button("Slice"))
        {
            _controller.Slice();
        }
    }

    void SpriteEditor::RenderGridSequenceSlicingControls()
    {
        ImGui::DragInt(
            "Slice start",
            &_controller.SliceStart,
            1,
            0,
            _controller.SliceEnd - 1,
            "%d",
            _controller.SliceEnd == 1 ? ImGuiSliderFlags_ReadOnly : ImGuiSliderFlags_None
        );
        ImGui::DragInt(
            "Slice end", 
            &_controller.SliceEnd, 
            1,
            _controller.SliceStart, 
            _controller.GridWidth * _controller.GridHeight
        );
    }

    void SpriteEditor::RenderSprites()
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

            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
            {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    _controller.SelectedSpriteId = i;
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    void SpriteEditor::RenderNewSpriteAtlasModal()
    {
        bool isOpen = true;
        if (ImGui::NewSpriteAtlas(_spriteAtlasData, isOpen))
        {
            _controller.CreateNewSpriteAtlas(_spriteAtlasData);
        }
    }

    void SpriteEditor::RenderSelectedSprite()
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
