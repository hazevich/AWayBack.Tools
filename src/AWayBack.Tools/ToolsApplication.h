#pragma once

#include "Application.h"

#include "UndoRedo.h"

#include "UI/NewSpriteAtlasModal.h"
#include "UI/CanvasWindow.h"
#include "UI/SelectedSpriteWindow.h"
#include "UI/ToolbarWindow.h"
#include "UI/SpritesWindow.h"
#include "UI/NewAnimationAtlasModal.h"

namespace AWayBack
{
    class ToolsApplication : public Application
    {
    public:
        ToolsApplication();

        UndoRedoHistory UndoRedoHistory;

    protected:
        void Render() override;

    private:
        SpriteAtlasEditorController _spriteAtlasEditorController;
        UI::AnimationEditor::AnimatorController _animatorController;

        CanvasWindow _canvas;
        ToolbarWindow _toolbar;
        SpritesWindow _sprites;
        SelectedSpriteWindow _selectedSprite;
        NewSpriteAtlasModal _newSpriteAtlasModal;
        UI::AnimationEditor::NewAnimationAtlasModal _newAnimationAtlasModal;

        void RenderDockSpace();
        void RenderMainMenuBar();
    };
}
