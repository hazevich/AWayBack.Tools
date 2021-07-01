#include "SpriteEditor.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiExt.h"
#include "algorithm"
#include "SpriteAtlasSerializer.h"
#include "fstream"

namespace fs = std::filesystem;

namespace AWayBack
{
    SpriteEditor::~SpriteEditor()
    {
        delete _newTexture;
        delete _texture;
        delete _spriteAtlas;
    }

    void SpriteEditor::LoadTexture(const std::string& texturePath)
    {
        delete _newTexture;
        _newTexture = Texture2D::FromFile(texturePath);

        delete _spriteAtlas;
        _spriteAtlas = new SpriteAtlas { "", _newTexture->GetName(), _newTexture->GetName(), std::vector<Sprite>() };
    }

    void SpriteEditor::CreateNewSpriteAtlas()
    {
        _isNewSpriteAtlasRequested = true;
        _newSpriteAtlas = new SpriteAtlas();
    }

    void SpriteEditor::LoadSpriteAtlas(const std::string& spriteAtlasPath)
    {
        std::ifstream fileStream;
        fileStream.open(spriteAtlasPath);
        std::optional<SpriteAtlas*> spriteAtlas = SpriteAtlasSerializer::DeserializeFromFile(fileStream);
        if (spriteAtlas)
        {
            delete _spriteAtlas;
            _spriteAtlas = spriteAtlas.value();
            fs::path path = spriteAtlasPath;
            auto spriteAtlasName = path.filename().string();
            auto folderPath = path.parent_path().string();
            _spriteAtlas->Path = folderPath;
            _spriteAtlas->Name = spriteAtlasName;

            delete _newTexture;
            _newTexture = Texture2D::FromFile(_spriteAtlas->TextureName);
        }
    }

    void SpriteEditor::Render()
    {
        if (_isNewSpriteAtlasRequested)
        {
            ImGui::OpenPopup("New spritesheet");
            _isNewSpriteAtlasRequested = false;
        }

        SyncData();

        RenderCanvas();
        RenderControls();
        RenderSprites();
        RenderSelectedSprite();
        RenderNewSpriteAtlasModal();
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

            auto cellSize = _cellSize,
                 gridSize = ImGui::ImVec2i(_texture->GetWidth(), _texture->GetHeight());

            ImGui::CheckerBoard(cellSize, gridSize);

            ImGui::Image(*_texture);

            ImGui::Grid(cursorScreenPos, cellSize, gridSize);

            ImDrawList* drawList = ImGui::GetWindowDrawList();

            for (int32_t i = _sliceStart; i < _sliceEnd; i++)
            {
                int32_t x = i % _gridWidth * _cellSize.X + cursorScreenPos.x;
                int32_t y = i / _gridWidth * _cellSize.Y + cursorScreenPos.y;

                drawList->AddRect(ImVec2(x, y), ImVec2(x + _cellSize.X, y + _cellSize.Y), ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)));
            }

            if (_selectedSpriteId)
            {
                int32_t value = _selectedSpriteId.value();
                if (_spriteAtlas->Sprites.size() > value)
                {
                    Sprite& sprite = _spriteAtlas->Sprites[value];

                    drawList->AddRectFilled(
                        ImVec2(cursorScreenPos.x + sprite.Min.X, cursorScreenPos.y + sprite.Min.Y), 
                        ImVec2(cursorScreenPos.x + sprite.Max.X, cursorScreenPos.y + sprite.Max.Y), 
                        ImGui::GetColorU32({0.0f, 0.0f, 1.0f, 0.3f})
                    );
                }
            }
        }

        ImGui::EndChild();

        ImGui::End();
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

        ImGui::LabelText("Sprite Atlas Name", _spriteAtlas->Name.c_str());
        ImGui::NewLine();
        ImGui::LabelText("Texture name", _spriteAtlas->TextureName.c_str());

        if (ImGui::Button("Save"))
        {
            auto spriteAtlasPath = fs::path(_spriteAtlas->Path) / fs::path(_spriteAtlas->Name);

            std::ofstream file;
            file.open(spriteAtlasPath);
            SpriteAtlasSerializer::SerializeToFile(file, *_spriteAtlas);
            file.close();
        }
    }

    void SpriteEditor::RenderSlicingControls()
    {
        if (!ImGui::CollapsingHeader("Slicing", ImGuiTreeNodeFlags_DefaultOpen)) return;

        if (_isUniformCellSizeControl)
        {
            if (ImGui::DragInt("##Cell size", &_cellSize.X, 1, 1, INT32_MAX))
            {
                _cellSize.Y = _cellSize.X;

                CalculateGridSize();
            }
        }
        else if (ImGui::DragInt2("##Cell size", _cellSize.Components, 1, 1, INT32_MAX))
        {
            CalculateGridSize();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cell size"))
        {
            _isUniformCellSizeControl = !_isUniformCellSizeControl;
        }

        RenderGridSequenceSlicingControls();

        if (ImGui::Button("Slice"))
        {
            for (int32_t i = _sliceStart; i < _sliceEnd; i++)
            {
                int32_t x = i % _gridWidth * _cellSize.X;
                int32_t y = i / _gridWidth * _cellSize.Y;

                auto min = Vector2(x, y);
                auto max = Vector2(x + _cellSize.X, y + _cellSize.Y);
                auto name = _spriteAtlas->Name + std::to_string(_spriteAtlas->Sprites.size());
                Sprite sprite = { name, min, max, Vector2() };
                _spriteAtlas->Sprites.push_back(sprite);
            }
        }
    }

    void SpriteEditor::RenderGridSequenceSlicingControls()
    {
        ImGui::DragInt("Slice start", &_sliceStart, 1, 0, _sliceEnd - 1, "%d",
                       _sliceEnd == 1 ? ImGuiSliderFlags_ReadOnly : ImGuiSliderFlags_None);
        ImGui::DragInt("Slice end", &_sliceEnd, 1, _sliceStart, _gridWidth * _gridHeight);
    }

    void SpriteEditor::RenderSprites()
    {
        const int32_t itemSize = 70;

        if (!ImGui::Begin("Sprites"))
        {
            ImGui::End();
            return;
        }

        char childTitleBuffer[300] = {0};

        for (int32_t i = 0; i < _spriteAtlas->Sprites.size(); i++)
        {
            Sprite& sprite = _spriteAtlas->Sprites[i];
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

            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
            {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    _selectedSpriteId = i;
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    void SpriteEditor::RenderNewSpriteAtlasModal()
    {
        bool isOpen = true;
        if (ImGui::NewSpriteAtlas(*_newSpriteAtlas, isOpen))
        {
            _spriteAtlas = _newSpriteAtlas;

            auto texturePath = fs::path(_spriteAtlas->Path) / fs::path(_spriteAtlas->TextureName);

            _newTexture = Texture2D::FromFile(texturePath.string());
            _newSpriteAtlas = nullptr;
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
        
        if (_selectedSpriteId)
        {
            Sprite& sprite = _spriteAtlas->Sprites[_selectedSpriteId.value()];
            Vector2 spriteSize =  sprite.Max - sprite.Min;
            auto spritePosition = ImVec2((contentRegionAvail.x - spriteSize.X) * 0.5f, (contentRegionAvail.y - spriteSize.Y) * 0.5f);
            ImGui::SetCursorPos(spritePosition);

            ImGui::CheckerBoard(_cellSize, ImGui::ImVec2i(spriteSize.X, spriteSize.Y));

            auto spriteuv0 = ImVec2(sprite.Min.X / _texture->GetWidth(), sprite.Min.Y / _texture->GetHeight());
            auto spriteuv1 = ImVec2(sprite.Max.X / _texture->GetWidth(), sprite.Max.Y / _texture->GetHeight());
            ImVec2 imageScreenPos = ImGui::GetCursorScreenPos();
            ImGui::Image(*_texture, ImVec2(spriteSize.X, spriteSize.Y), spriteuv0, spriteuv1);

            const float borderOffset = 5;
            ImGui::Border(ImVec2(imageScreenPos.x - borderOffset, imageScreenPos.y - borderOffset), ImVec2(spriteSize.X + borderOffset * 2, spriteSize.Y + borderOffset * 2), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)));
        }
        else
        {
            ImGui::CheckerBoard(_cellSize, ImGui::ImVec2i(contentRegionAvail.x, contentRegionAvail.y));            
        }
        
        ImGui::End();
    }

    void SpriteEditor::CalculateGridSize()
    {
        _gridWidth = _texture->GetWidth() / _cellSize.X;
        _gridHeight = _texture->GetHeight() / _cellSize.Y;
        _sliceStart = _sliceEnd = 0;
    }

    void SpriteEditor::SyncData()
    {
        if (_newTexture)
        {
            delete _texture;
            _texture = _newTexture;
            _newTexture = nullptr;

            _sliceStart = 0;
            _sliceEnd = 0;

            CalculateGridSize();
        }
    }

}
