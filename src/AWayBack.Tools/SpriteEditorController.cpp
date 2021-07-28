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

    struct ClearSpritesCommand : UndoRedoCommand
    {
        ClearSpritesCommand(SpriteAtlas& spriteAtlas, std::optional<int32_t>& selectedSpriteId)
            : _spriteAtlas(spriteAtlas), _selectedSpriteId(selectedSpriteId), _previousSpriteId(_selectedSpriteId)
        {
        }

        void Execute() override
        {
            _clearedSprites.insert(_clearedSprites.begin(), _spriteAtlas.Sprites.begin(), _spriteAtlas.Sprites.end());
            _spriteAtlas.Sprites.clear();

            _selectedSpriteId = std::nullopt;
        }

        void Undo() override
        {
            _spriteAtlas.Sprites.insert(_spriteAtlas.Sprites.begin(), _clearedSprites.begin(), _clearedSprites.end());
            _clearedSprites.clear();

            _selectedSpriteId = _previousSpriteId;
        }

    private:
        SpriteAtlas& _spriteAtlas;
        std::optional<int32_t>& _selectedSpriteId;

        std::optional<int32_t> _previousSpriteId = std::nullopt;
        std::vector<Sprite> _clearedSprites{};
    };

    void SliceSprite(SpriteAtlas& spriteAtlas, int32_t cell, ImVec2i cellSize, int32_t gridWidth)
    {
        ImVec2 position = GetPositonFromCell(cell, cellSize, gridWidth);

        auto min = Vector2(position.x, position.y);
        auto max = Vector2(position.x + cellSize.X, position.y + cellSize.Y);
        auto name = spriteAtlas.Name + std::to_string(spriteAtlas.Sprites.size());
        Sprite sprite = {name, min, max, Vector2()};
        spriteAtlas.Sprites.push_back(sprite);
    }

    struct SliceGridSequenceCommand : UndoRedoCommand
    {
        SliceGridSequenceCommand(
            int32_t& sliceStart,
            int32_t& sliceEnd,
            SpriteAtlas& spriteAtlas,
            ImVec2i cellSize,
            int32_t gridWidth,
            SlicingType& slicingType
        )
            : _sliceStart(sliceStart),
              _sliceEnd(sliceEnd),
              _spriteAtlas(spriteAtlas),
              _cellSize(cellSize),
              _gridWidth(gridWidth),
              _slicingType(slicingType),
             _previousSliceStart(_sliceStart),
             _previousSliceEnd(_sliceEnd)
        {
        }

        void Execute() override
        {
            _newSpritesBeginIndex = _spriteAtlas.Sprites.size();
            _newSpritesCount = _sliceEnd - _sliceStart;

            for (int32_t i = _sliceStart; i < _sliceEnd; i++)
            {
                SliceSprite(_spriteAtlas, i, _cellSize, _gridWidth);
            }
            
            _sliceStart = _sliceEnd = 0;
        }

        void Undo() override
        {
            auto first = _spriteAtlas.Sprites.begin() + _newSpritesBeginIndex;
            auto last = first + _newSpritesCount;
            _spriteAtlas.Sprites.erase(first, last);

            _sliceStart = _previousSliceStart;
            _sliceEnd = _previousSliceEnd;

            _slicingType = SlicingType::GridSequence;
        }

    private:
        int32_t& _sliceStart;
        int32_t& _sliceEnd;
        SpriteAtlas& _spriteAtlas;
        ImVec2i _cellSize;
        int32_t _gridWidth;
        SlicingType& _slicingType;

        int32_t _previousSliceStart;
        int32_t _previousSliceEnd;

        int32_t _newSpritesBeginIndex;
        int32_t _newSpritesCount;
    };

    struct SliceGridSelectionCommand : UndoRedoCommand
    {
        SliceGridSelectionCommand(
            std::vector<int32_t>& selectedCells,
            SpriteAtlas& spriteAtlas,
            ImVec2i cellSize,
            int32_t gridWidth,
            SlicingType& slicingType
        )
            : _selectedCells(selectedCells),
              _spriteAtlas(spriteAtlas),
              _cellSize(cellSize),
              _gridWidth(gridWidth),
              _slicingType(slicingType)
        {
            _previouslySelectedCells.insert(_previouslySelectedCells.begin(), _selectedCells.begin(), _selectedCells.end());
        }

        void Execute() override
        {
            _newSpritesBeginIndex = _spriteAtlas.Sprites.size();
            _newSpritesCount = _selectedCells.size();

            for (int32_t i : _selectedCells)
            {
                SliceSprite(_spriteAtlas, i, _cellSize, _gridWidth);
            }
            
            _selectedCells.clear();
        }

        void Undo() override
        {
            auto first = _spriteAtlas.Sprites.begin() + _newSpritesBeginIndex;
            auto last = first + _newSpritesCount;
            _spriteAtlas.Sprites.erase(first, last);

            _selectedCells.insert(_selectedCells.begin(), _previouslySelectedCells.begin(), _previouslySelectedCells.end());

            _slicingType = SlicingType::GridSelection;
        }

    private:
        std::vector<int32_t>& _selectedCells;
        SpriteAtlas& _spriteAtlas;
        ImVec2i _cellSize;
        int32_t _gridWidth;
        SlicingType& _slicingType;

        std::vector<int32_t> _previouslySelectedCells;
        int32_t _newSpritesBeginIndex;
        int32_t _newSpritesCount;
    };

    struct SliceFreehandCommand : UndoRedoCommand
    {
        SliceFreehandCommand(SelectedRegion& selectedRegion, SpriteAtlas& spriteAtlas, SlicingType& slicingType)
            : _selectedRegion(selectedRegion), _spriteAtlas(spriteAtlas), _slicingType(slicingType), _previouslySelectedRegion(selectedRegion)
        {
        }

        void Execute() override
        {
            auto min = Vector2(_selectedRegion.Min.x, _selectedRegion.Min.y);
            auto max = Vector2(_selectedRegion.Max.x, _selectedRegion.Max.y);
            auto name = _spriteAtlas.Name + std::to_string(_spriteAtlas.Sprites.size());
            Sprite sprite = {name, min, max, Vector2()};
            _spriteAtlas.Sprites.push_back(sprite);

            _selectedRegion = SelectedRegion();
        }

        void Undo() override
        {
            _spriteAtlas.Sprites.pop_back();
            _selectedRegion = _previouslySelectedRegion;

            _slicingType = SlicingType::Freehand;
        }

    private:
        SelectedRegion& _selectedRegion;
        SpriteAtlas& _spriteAtlas;
        SlicingType& _slicingType;

        SelectedRegion _previouslySelectedRegion;
    };

    struct RenameSpriteCommand : UndoRedoCommand
    {
        RenameSpriteCommand(const std::string& spriteName, Sprite& sprite)
            : _spriteName(spriteName),
              _previousSpriteName(sprite.Name),        
              _sprite(sprite)
        {
            
        }

        void Execute() override
        {
            _sprite.Name = _spriteName;
        }

        void Undo() override
        {
            _sprite.Name = _previousSpriteName;
        }
        
    private:
        const std::string _spriteName;
        const std::string _previousSpriteName;

        Sprite& _sprite;
    };

    struct SetOriginForAllSpritesCommand : UndoRedoCommand
    {
        SetOriginForAllSpritesCommand(Vector2 newOrigin, std::vector<Sprite>& sprites)
            : _newOrigin(newOrigin),
              _sprites(sprites)   
        {
            for (Sprite& sprite : _sprites)
                _previousOrigins.push_back(sprite.Origin);
        }

        void Execute() override
        {
            for (Sprite& sprite : _sprites)
                sprite.Origin = _newOrigin;
        }

        void Undo() override
        {
            for (int32_t i = 0; i < _previousOrigins.size(); i++)
            {
                _sprites[i].Origin = _previousOrigins[i];
            }
        }

    private:
        Vector2 _newOrigin;
        std::vector<Sprite>& _sprites;
        std::vector<Vector2> _previousOrigins;
    };

    struct SetOriginCommand : UndoRedoCommand
    {
        SetOriginCommand(Vector2 origin, Sprite& sprite)
            : _newOrigin(origin), _previousOrigin(sprite.Origin), _sprite(sprite)
        {   
        }

        void Execute() override
        {
            _sprite.Origin = _newOrigin;
        }

        void Undo() override
        {
            _sprite.Origin = _previousOrigin;
        }

    private:
        const Vector2 _newOrigin;
        const Vector2 _previousOrigin;
        Sprite& _sprite;
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
        switch (SlicingType)
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

    void SpriteEditorController::SliceGridSequence()
    {
        _undoRedoHistory.AddCommand(
            new SliceGridSequenceCommand(SliceStart, SliceEnd, *_spriteAtlas, _cellSize, GridWidth, SlicingType));
    }

    void SpriteEditorController::SliceGridSelection()
    {
        _undoRedoHistory.AddCommand(
            new SliceGridSelectionCommand(SelectedCells, *_spriteAtlas, _cellSize, GridWidth, SlicingType));
    }

    void SpriteEditorController::SliceFreehand()
    {
        _undoRedoHistory.AddCommand(new SliceFreehandCommand(SelectedRegion, *_spriteAtlas, SlicingType));
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
        _undoRedoHistory.AddCommand(new ClearSpritesCommand(*_spriteAtlas, SelectedSpriteId));
    }

    void SpriteEditorController::CalculateGridSize()
    {
        if (!_texture) return;

        GridWidth = _texture->GetWidth() / _cellSize.X;
        GridHeight = _texture->GetHeight() / _cellSize.Y;
    }

    int32_t GetCellFromPosition(ImVec2 position, ImVec2i cellSize, int32_t gridWidth)
    {
        int32_t cellX = (int32_t)position.x / cellSize.X;
        int32_t cellY = (int32_t)position.y / cellSize.Y;

        return cellY * gridWidth + cellX;
    }

    ImVec2 GetPositonFromCell(int32_t cell, ImVec2i cellSize, int32_t gridWidth)
    {
        int32_t x = cell % gridWidth * cellSize.X;
        int32_t y = cell / gridWidth * cellSize.Y;

        return ImVec2(x, y);
    }

    void SpriteEditorController::RenameSelectedSprite(const std::string& name)
    {
        if (!SelectedSpriteId) return;

        Sprite& sprite = _spriteAtlas->Sprites[SelectedSpriteId.value()];

        if (sprite.Name != name)
            _undoRedoHistory.AddCommand(new RenameSpriteCommand(name, sprite));
    }

    void SpriteEditorController::SetOriginForSelectedSprite(const Vector2 origin)
    {
        if (!SelectedSpriteId) return;

        Sprite& sprite = _spriteAtlas->Sprites[SelectedSpriteId.value()];

        _undoRedoHistory.AddCommand(new SetOriginCommand(origin, sprite));
    }

    void SpriteEditorController::SetOriginForAllSprites(Vector2 origin)
    {
        _undoRedoHistory.AddCommand(new SetOriginForAllSpritesCommand(origin, _spriteAtlas->Sprites));
    }
}
