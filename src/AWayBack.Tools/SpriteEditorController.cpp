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
              _removedSprite(spriteAtlas.Sprites[spriteIndex])
        {
        }

        void Undo() override
        {
            _spriteAtlas.Sprites.insert(_spriteAtlas.Sprites.begin() + _spriteIndex, _removedSprite.value());
        }

        void Execute() override
        {
            if (_spriteIndex == _selectedSpriteId) _selectedSpriteId = std::nullopt;

            _spriteAtlas.Sprites.erase(_spriteAtlas.Sprites.begin() + _spriteIndex);
        }

        const char* GetType() override
        {
            return "Remove sprite command";
        }

    private:
        const int32_t _spriteIndex;
        SpriteAtlas& _spriteAtlas;
        std::optional<int32_t> _selectedSpriteId;
        const std::optional<Sprite> _removedSprite;
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

        const char* GetType() override
        {
            return "Clear sprites command";
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
            SlicingType& slicingType,
            std::optional<int32_t>& selectedSpriteId
        )
            : _sliceStart(sliceStart),
              _sliceEnd(sliceEnd),
              _spriteAtlas(spriteAtlas),
              _cellSize(cellSize),
              _gridWidth(gridWidth),
              _slicingType(slicingType),
              _selectedSpriteId(selectedSpriteId),
              _previousSliceStart(_sliceStart),
              _previousSliceEnd(_sliceEnd),
              _newSpritesBeginIndex(_spriteAtlas.Sprites.size()),
              _newSpritesCount(sliceEnd - _sliceStart)
        {
        }

        void Execute() override
        {
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
            _selectedSpriteId = std::nullopt;
        }

        const char* GetType() override
        {
            return "Slice grid sequence";
        }

    private:
        int32_t& _sliceStart;
        int32_t& _sliceEnd;
        SpriteAtlas& _spriteAtlas;
        ImVec2i _cellSize;
        int32_t _gridWidth;
        SlicingType& _slicingType;
        std::optional<int32_t>& _selectedSpriteId;

        const int32_t _previousSliceStart;
        const int32_t _previousSliceEnd;

        const int32_t _newSpritesBeginIndex;
        const int32_t _newSpritesCount;
    };

    struct SliceGridSelectionCommand : UndoRedoCommand
    {
        SliceGridSelectionCommand(
            std::vector<int32_t>& selectedCells,
            SpriteAtlas& spriteAtlas,
            ImVec2i cellSize,
            int32_t gridWidth,
            SlicingType& slicingType,
            std::optional<int32_t>& selectedSpriteId
        )
            : _selectedCells(selectedCells),
              _spriteAtlas(spriteAtlas),
              _cellSize(cellSize),
              _gridWidth(gridWidth),
              _slicingType(slicingType),
              _selectedSpriteId(selectedSpriteId),
              _newSpritesBeginIndex(_spriteAtlas.Sprites.size()),
              _newSpritesCount(_selectedCells.size()),
              _previouslySelectedCells(_selectedCells)

        {
        }

        void Execute() override
        {
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
            _selectedSpriteId = std::nullopt;
        }

        const char* GetType() override
        {
            return "Slice grid selection";
        }

    private:
        std::vector<int32_t>& _selectedCells;
        SpriteAtlas& _spriteAtlas;
        ImVec2i _cellSize;
        int32_t _gridWidth;
        SlicingType& _slicingType;
        std::optional<int32_t>& _selectedSpriteId;

        const std::vector<int32_t> _previouslySelectedCells;
        const int32_t _newSpritesBeginIndex;
        const int32_t _newSpritesCount;
    };

    struct SliceFreehandCommand : UndoRedoCommand
    {
        SliceFreehandCommand(
            SelectedRegion& selectedRegion, 
            SpriteAtlas& spriteAtlas, 
            SlicingType& slicingType,
            std::optional<int32_t>& selectedSpriteId)
            : _selectedRegion(selectedRegion),
              _spriteAtlas(spriteAtlas),
              _slicingType(slicingType),
              _selectedSpriteId(selectedSpriteId),
              _previouslySelectedRegion(selectedRegion)
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
            _selectedSpriteId = std::nullopt;
        }

        const char* GetType() override
        {
            return "Slice freehand";
        }

    private:
        SelectedRegion& _selectedRegion;
        SpriteAtlas& _spriteAtlas;
        SlicingType& _slicingType;
        std::optional<int32_t>& _selectedSpriteId;

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

        const char* GetType() override
        {
            return "Rename sprite";
        }

        bool Merge(UndoRedoCommand& other) override
        {
            auto& otherRenameSprite = dynamic_cast<RenameSpriteCommand&>(other);

            if (&_sprite != &otherRenameSprite._sprite) return false;

            _spriteName = otherRenameSprite._spriteName;

            return true;
        }
        
    private:
        std::string _spriteName;
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

        const char* GetType() override
        {
            return "Set origin for all sprites";
        }

    private:
        Vector2 _newOrigin;
        std::vector<Sprite>& _sprites;
        std::vector<Vector2> _previousOrigins;
    };

    struct SetSpriteOriginCommand : UndoRedoCommand
    {
        SetSpriteOriginCommand(Vector2 origin, Sprite& sprite)
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

        const char* GetType() override
        {
            return "Set origin";
        }

        bool Merge(UndoRedoCommand& other) override
        {
            auto& otherSpriteOriginCommand = dynamic_cast<SetSpriteOriginCommand&>(other);

            if (&_sprite != &otherSpriteOriginCommand._sprite) return false;

            _newOrigin = otherSpriteOriginCommand._newOrigin;

            return true;
        }

    private:
        Vector2 _newOrigin;
        const Vector2 _previousOrigin;
        Sprite& _sprite;
    };

    struct SetSpritePositionCommand : UndoRedoCommand
    {
        SetSpritePositionCommand(const Vector2& min, Sprite& sprite)
            : _min(min),
              _max(_min + (sprite.Max - sprite.Min)),
              _previousMin(sprite.Min),
              _previousMax(sprite.Max),
              _sprite(sprite)
        {
        }

        void Execute() override
        {
            _sprite.Min = _min;
            _sprite.Max = _max;
        }

        void Undo() override
        {
            _sprite.Min = _previousMin;
            _sprite.Max = _previousMax;
        }
        
        bool Merge(UndoRedoCommand& other) override
        {
            auto& otherMinMaxCommand = dynamic_cast<SetSpritePositionCommand&>(other);
            
            if (&otherMinMaxCommand._sprite != &_sprite) return false;

            _min = otherMinMaxCommand._min;
            _max = otherMinMaxCommand._max;

            return true;
        }

        const char* GetType() override
        {
            return "Set sprite position";
        }
    private:
        Vector2 _min;
        Vector2 _max;

        const Vector2 _previousMin;
        const Vector2 _previousMax;

        Sprite& _sprite;
    };

    struct SetSpriteSizeCommand : UndoRedoCommand
    {
        SetSpriteSizeCommand(Vector2 spriteSize, Sprite& sprite)
            : _spriteSize(spriteSize), _previousMax(sprite.Max), _sprite(sprite)
        {   
        }

        void Execute() override
        {
            _sprite.Max = _sprite.Min + _spriteSize;
        }

        void Undo() override
        {
            _sprite.Max = _previousMax;
        }

        const char* GetType() override
        {
            return "Set sprite size";
        }

        bool Merge(UndoRedoCommand& other) override
        {
            auto& otherSetSpriteSizeCommand = dynamic_cast<SetSpriteSizeCommand&>(other);

            if (&_sprite != &otherSetSpriteSizeCommand._sprite) return false;

            _spriteSize = otherSetSpriteSizeCommand._spriteSize;

            return true;
        }

    private:
        Vector2 _spriteSize;
        const Vector2 _previousMax;
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

    bool SpriteEditorController::CanSlice()
    {
        switch (SlicingType)
        {
        case SlicingType::GridSequence:
            return SliceStart != SliceEnd;
        case SlicingType::GridSelection:
            return !SelectedCells.empty();
        case SlicingType::Freehand:
            return SelectedRegion.Min.x < SelectedRegion.Max.x
                   && SelectedRegion.Min.y < SelectedRegion.Max.y;
        }

        return false;
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
        _undoRedoHistory.ExecuteCommand(
            new SliceGridSequenceCommand(SliceStart, SliceEnd, *_spriteAtlas, _cellSize, GridWidth, SlicingType, SelectedSpriteId));
    }

    void SpriteEditorController::SliceGridSelection()
    {
        _undoRedoHistory.ExecuteCommand(
            new SliceGridSelectionCommand(SelectedCells, *_spriteAtlas, _cellSize, GridWidth, SlicingType, SelectedSpriteId));
    }

    void SpriteEditorController::SliceFreehand()
    {
        _undoRedoHistory.ExecuteCommand(new SliceFreehandCommand(SelectedRegion, *_spriteAtlas, SlicingType, SelectedSpriteId));
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
        _undoRedoHistory.ExecuteCommand(new RemoveSpriteCommand(spriteIndex, *_spriteAtlas, SelectedSpriteId));
    }

    void SpriteEditorController::ClearSprites()
    {
        _undoRedoHistory.ExecuteCommand(new ClearSpritesCommand(*_spriteAtlas, SelectedSpriteId));
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

    void SpriteEditorController::RenameSprite(int32_t spriteId, const std::string& name)
    {
        Sprite& sprite = _spriteAtlas->Sprites[spriteId];

        if (sprite.Name != name)
            _undoRedoHistory.ExecuteCommand(new RenameSpriteCommand(name, sprite));
    }

    void SpriteEditorController::SetSpriteOrigin(int32_t spriteId, Vector2 origin, bool isFinalEdit)
    {
        Sprite& sprite = _spriteAtlas->Sprites[spriteId];

        _undoRedoHistory.ExecuteCommand(new SetSpriteOriginCommand(origin, sprite), isFinalEdit);
    }

    void SpriteEditorController::SetOriginForAllSprites(Vector2 origin)
    {
        _undoRedoHistory.ExecuteCommand(new SetOriginForAllSpritesCommand(origin, _spriteAtlas->Sprites));
    }

    Vector2 CalculateOrigin(Sprite& sprite, OriginPlacement originPlacement)
    {
        Vector2 spriteSize = sprite.Max - sprite.Min;

        switch (originPlacement)
        {
        case Center:
            return Vector2(spriteSize.X / 2, spriteSize.Y / 2);
        case TopLeft:
            return Vector2(0, 0);
        case TopCenter:
            return Vector2(spriteSize.X / 2, 0);
        case TopRight:
            return Vector2(spriteSize.X, 0);
        case CenterRight:
            return Vector2(spriteSize.X, spriteSize.Y / 2);
        case BottomRight:
            return spriteSize;
        case BottomCenter:
            return Vector2(spriteSize.X / 2, spriteSize.Y);
        case BottomLeft:
            return Vector2(0, spriteSize.Y);
        case LeftCenter:
            return Vector2(0, spriteSize.Y / 2);
        case Custom:
            return sprite.Origin;
        }

        return sprite.Origin;
    }

    void SpriteEditorController::SetSpriteOrigin(int32_t spriteId, AWayBack::OriginPlacement originPlacement)
    {
        Sprite& sprite = _spriteAtlas->Sprites[spriteId];
        Vector2 origin = CalculateOrigin(sprite, originPlacement);
        _undoRedoHistory.ExecuteCommand(new SetSpriteOriginCommand(origin, sprite));

        OriginPlacement = originPlacement;
    }

    const SpriteAtlas& SpriteEditorController::GetSpriteAtlas() const
    {
        return *_spriteAtlas;
    }

    const Sprite& SpriteEditorController::GetSprite(int32_t spriteId) const
    {
        return _spriteAtlas->Sprites[spriteId];
    }

    void SpriteEditorController::SetSpritePosition(int32_t spriteId, Vector2 position, bool isFinalEdit)
    {
        Sprite& sprite = _spriteAtlas->Sprites[spriteId];
        _undoRedoHistory.ExecuteCommand(new SetSpritePositionCommand(position, sprite), isFinalEdit);
    }

    void SpriteEditorController::SetSpriteSize(int32_t spriteId, Vector2 spriteSize, bool isFinalEdit)
    {
        Sprite& sprite = _spriteAtlas->Sprites[spriteId];

        _undoRedoHistory.ExecuteCommand(new SetSpriteSizeCommand(spriteSize, sprite), isFinalEdit);
    }
}
