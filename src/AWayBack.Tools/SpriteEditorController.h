#pragma once
#include "SpriteAtlas.h"
#include "ImGuiExt.h"

#include "optional"

#include "AWayBack/Graphics/Texture.h"

namespace AWayBack
{
    class SpriteEditorController
    {
    public:
        ~SpriteEditorController();

        int32_t SliceStart, SliceEnd;
        int32_t GridWidth, GridHeight;

        std::optional<int32_t> SelectedSpriteId = std::nullopt;

        ImGui::ImVec2i GetCellSize() { return _cellSize; }

        SpriteAtlas& GetSpriteAtlas() { return *_spriteAtlas; }

        Texture2D* GetTexture() { return _texture; }

        void LoadSpriteAtlas(const std::string& spriteAtlasPath);
        void CreateNewSpriteAtlas(SpriteAtlasData& spriteAtlasData);

        void Slice();
        void Save();

        void SetCellSize(const ImGui::ImVec2i& cellSize);

    private:
        SpriteAtlas* _spriteAtlas = new SpriteAtlas();

        Texture2D* _texture = nullptr;

        ImGui::ImVec2i _cellSize = ImGui::ImVec2i(32, 32);

        void CalculateGridSize();
    };
}
