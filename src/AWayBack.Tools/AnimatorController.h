#pragma once

#include "AnimationAtlas.h"
#include "SpriteAtlas.h"
#include <string>

namespace AWayBack::UI::AnimationEditor
{
    class AnimatorController
    {
    public:
        uint8_t SelectedAnimationIndex;
        uint8_t BeginFrame, EndFrame;
        float FrameDuration;
        bool IsRepeatable;

        void CreateNewAnimationAtlas(const AnimationAtlasData& animationAtlasData);
        const Animation& CreateAnimation();
        [[nodiscard]] const std::vector<Animation>& GetAnimations() const;
        [[nodiscard]] const Animation& GetAnimation(uint8_t animationIndex) const;
        [[nodiscard]] const AnimationAtlas* GetAnimationAtlas() const;

    private:
        AnimationAtlas* _animationAtlas = nullptr;
    };
}