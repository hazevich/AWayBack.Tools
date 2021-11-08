#pragma once
#include "SpriteAtlasEditorController.h"
#include "AnimatorController.h"

namespace AWayBack
{
    struct ToolbarWindow
    {
        ToolbarWindow(SpriteAtlasEditorController& controller, UndoRedoHistory& undoRedoHistory)
            : _controller(controller), _undoRedoHistory(undoRedoHistory)
        {
        }

        void Render();

    private:
        SpriteAtlasEditorController& _controller;
        UndoRedoHistory& _undoRedoHistory;
        bool _isUniformCellSizeControl = true;
    };

    namespace UI::AnimationEditor
    {
        struct AnimatorToolbarWindow
        {
            AnimatorToolbarWindow(AnimatorController& animatorController);

            void Render();

        private:
            AnimatorController& _animatorController;
        };
    }
}
