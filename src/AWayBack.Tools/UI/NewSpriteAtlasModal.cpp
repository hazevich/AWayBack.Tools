#include "NewSpriteAtlasModal.h"
#include "imgui.h"
#include "AtlasControls.h"

namespace AWayBack
{
    void NewSpriteAtlasModal::Open()
    {
        _isOpenRequested = true;
    }

    bool NewSpriteAtlas(const char* name, SpriteAtlasData& spriteAtlas, bool& isOpen)
    {
        auto content = [&spriteAtlas]() -> void
        {
            UI::AtlasPathControls("SpriteAtlasPath", spriteAtlas.Name, spriteAtlas.Folder);

            ImGui::NewLine();

            UI::BrowseControls("Texture", spriteAtlas.TexturePath, "Textures (*.png)\0*.png\0");
        };

        return UI::NewAtlas(name, isOpen, content);
    }

    void NewSpriteAtlasModal::Render()
    {
        const char* popupName = "New sprite atlas";

        if (_isOpenRequested)
        {
            ImGui::OpenPopup(popupName);
            _spriteAtlasData = {};

            _isOpen = true;
            _isOpenRequested = false;
        }

        if (NewSpriteAtlas(popupName, _spriteAtlasData, _isOpen))
        {
            _controller.CreateNewSpriteAtlas(_spriteAtlasData);
        }
    }
}
