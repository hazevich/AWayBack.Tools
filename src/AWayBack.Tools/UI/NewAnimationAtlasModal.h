#pragma once

#include "AnimatorController.h"

namespace AWayBack::UI::AnimationEditor
{
    class NewAnimationAtlasModal
    {
    public:
        NewAnimationAtlasModal(AnimatorController& animationController);

        void Open();
        void Render();

    private:
        bool _isOpen;
        bool _isOpenRequested;

        AnimatorController& _animatorController;
        AnimationAtlasData _animationAtlasData;
    };
}