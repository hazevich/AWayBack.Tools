#include "SpriteEditorController.h"
#include "SpriteAtlasSerializer.h"

#include "UndoRedo.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace AWayBack
{
    struct RemoveSpriteCommand : UndoRedoCommand
    {
        RemoveSpriteCommand(int32_t spriteIndex, SpriteAtlas& spriteAtlas, std::optional<int32_t>& selectedSpriteId)
            : _spriteIndex(spriteIndex),
              _spriteAtlas(spriteAtlas),
              _selectedSpriteId(selectedSpriteId),
              _removedSprite(std::nullopt)
        {
            
        }

        void Undo() override
        {
            if (!_removedSprite) return;

            _spriteAtlas.Sprites.insert(_spriteAtlas.Sprites.begin() + _spriteIndex, _removedSprite.value());
        }

        void Execute() override
        {
            _removedSprite = _spriteAtlas.Sprites[_spriteIndex];
            
            if (_spriteAtlas.Sprites.size() <= _spriteIndex) return;
            if (_spriteIndex == _selectedSpriteId) _selectedSpriteId = std::nullopt;

            _spriteAtlas.Sprites.erase(_spriteAtlas.Sprites.begin() + _spriteIndex);
        }

    private:
        int32_t _spriteIndex;
        SpriteAtlas& _spriteAtlas;
        std::optional<int32_t> _selectedSpriteId;
        std::optional<Sprite> _removedSprite;
    };

    void ClearSelections(SpriteEditorController& controller) 
    {
        controller.SelectedCells.clear();
        controller.SelectedRegion = SelectedRegion();
        controller.SelectedSpriteId = std::nullopt;
        controller.SliceStart = controller.SliceEnd = 0;
    }

    SpriteEditorController::SpriteEditorController(UndoRedoHistory& undoRedoHistory)
        : _undoRedoHistory(undoRedoHistory)
    {
        
    }
    
    SpriteEditorController::~SpriteEditorController()
    {
        delete _spriteAtlas;
        delete _texture;
    }

    void SpriteEditorController::LoadSpriteAtlas(const std::string& spriteAtlasPath)
    {
        std::ifstream fileStream;
        fileStream.open(spriteAtlasPath);
        SpriteAtlas* spriteAtlas = SpriteAtlasSerializer::DeserializeFromFile(fileStream);
        if (spriteAtlas)
        {
            delete _spriteAtlas;
            _spriteAtlas = spriteAtlas;
            fs::path path = spriteAtlasPath;
            auto spriteAtlasName = path.filename().string();
            auto folderPath = path.parent_path().string();
            _spriteAtlas->Folder = folderPath;
            _spriteAtlas->Name = spriteAtlasName;

            delete _texture;
            _texture = Texture2D::FromFile(_spriteAtlas->TextureName);

            CalculateGridSize();
            ClearSelections(*this);

            _undoRedoHistory.Reset();
        }
    }

    void SpriteEditorController::CreateNewSpriteAtlas(SpriteAtlasData& spriteAtlasData)
    {
        auto textureName = fs::path(spriteAtlasData.TexturePath).filename();
        fs::path destinationTexturePath = spriteAtlasData.Folder / textureName;

        if (destinationTexturePath != spriteAtlasData.TexturePath)
        {
            try
            {
                fs::copy(spriteAtlasData.TexturePath, destinationTexturePath.string(),
                         fs::copy_options::overwrite_existing);
            }
            catch (const std::filesystem::filesystem_error& e)
            {
                printf(e.what());
                printf("\n");

                return;
            }
        }

        delete _spriteAtlas;
        _spriteAtlas = new SpriteAtlas();

        _spriteAtlas->TextureName = textureName.string();
        _spriteAtlas->Name = spriteAtlasData.Name + ".atlas";

        delete _texture;
        _texture = Texture2D::FromFile(_spriteAtlas->TextureName);

        CalculateGridSize();
        ClearSelections(*this);

        _undoRedoHistory.Reset();
    }

    void SpriteEditorController::Slice()
    {
        switch(SlicingType)
        {
            case SlicingType::GridSequence:
            {
                SliceGridSequence();
                break;
            }
            case SlicingType::GridSelection:
            {
                SliceGridSelection();
                break;
            }
            case SlicingType::Freehand:
            {
                SliceFreehand();
                break;
            }
        }
    }

    void SliceSprite(SpriteAtlas& spriteAtlas, int32_t cell, ImVec2i cellSize, int32_t gridWidth)
    {
        ImVec2 position = GetPositonFromCell(cell, cellSize, gridWidth);

        auto min = Vector2(position.x, position.y);
        auto max = Vector2(position.x + cellSize.X, position.y + cellSize.Y);
        auto name = spriteAtlas.Name + std::to_string(spriteAtlas.Sprites.size());
        Sprite sprite = {name, min, max, Vector2()};
        spriteAtlas.Sprites.push_back(sprite);
    }

    void SpriteEditorController::SliceGridSequence()
    {
        for (int32_t i = SliceStart; i < SliceEnd; i++)
        {
            SliceSprite(*_spriteAtlas, i, _cellSize, GridWidth);
        }

        SliceStart = SliceEnd = 0;
    }

    void SpriteEditorController::SliceGridSelection()
    {
        for (int32_t i : SelectedCells)
        {
            SliceSprite(*_spriteAtlas, i, _cellSize, GridWidth);
        }

        SelectedCells.clear();
    }

    void SpriteEditorController::SliceFreehand()
    {
        auto min = Vector2(SelectedRegion.Min.x, SelectedRegion.Min.y);
        auto max = Vector2(SelectedRegion.Max.x, SelectedRegion.Max.y);
        auto name = _spriteAtlas->Name + std::to_string(_spriteAtlas->Sprites.size());
        Sprite sprite = {name, min, max, Vector2()};
        _spriteAtlas->Sprites.push_back(sprite);
    }

    void SpriteEditorController::Save()
    {
        auto spriteAtlasPath = fs::path(_spriteAtlas->Folder) / fs::path(_spriteAtlas->Name);

        std::ofstream file;
        file.open(spriteAtlasPath);
        SpriteAtlasSerializer::SerializeToFile(file, *_spriteAtlas);
        file.close();
    }

    void SpriteEditorController::SetCellSize(const ImVec2i& cellSize)
    {
        _cellSize = cellSize;
        CalculateGridSize();
        SliceStart = SliceEnd = 0;
        SelectedCells.clear();
    }

    void SpriteEditorController::RemoveSprite(int32_t spriteIndex)
    {
        _undoRedoHistory.AddCommand(new RemoveSpriteCommand(spriteIndex, *_spriteAtlas, SelectedSpriteId));
    }

    void SpriteEditorController::ClearSprites()
    {
        _spriteAtlas->Sprites.clear();
        SelectedSpriteId = std::nullopt;
    }

    void SpriteEditorController::CalculateGridSize()
    {
        if (!_texture) return;

        GridWidth = _texture->GetWidth() / _cellSize.X;
        GridHeight = _texture->GetHeight() / _cellSize.Y;
    }

    int32_t GetCellFromPosition(ImVec2 position, ImVec2i cellSize, int32_t gridWidth)
    {
        int32_t cellX = (int32_t) position.x / cellSize.X;
        int32_t cellY = (int32_t) position.y / cellSize.Y;

        return cellY * gridWidth + cellX;   
    }

    ImVec2 GetPositonFromCell(int32_t cell, ImVec2i cellSize, int32_t gridWidth)
    {
        int32_t x = cell % gridWidth * cellSize.X;
        int32_t y = cell / gridWidth * cellSize.Y;

        return ImVec2(x, y);
    }
}
