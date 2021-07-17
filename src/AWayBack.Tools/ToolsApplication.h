#pragma once

#include "Application.h"

#include "UI/NewSpriteAtlasModal.h"
#include "UI/CanvasWindow.h"
#include "UI/SelectedSpriteWindow.h"
#include "UI/SpriteEditorControlsWindow.h"
#include "UI/SpritesWindow.h"

namespace AWayBack
{
    class ToolsApplication : public Application
    {
    public:
        ToolsApplication();

    protected:
        void Render() override;

    private:
        SpriteEditorController _spriteEditorController{};

        CanvasWindow _canvas;
        SpriteEditorControlsWindow _controls;
        SpritesWindow _sprites;
        SelectedSpriteWindow _selectedSprite;
        NewSpriteAtlasModal _newSpriteAtlasModal;

        void RenderDockSpace();
        void RenderMainMenuBar();
    };
}
