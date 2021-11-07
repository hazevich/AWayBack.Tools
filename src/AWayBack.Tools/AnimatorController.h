#pragma once

#include "AnimationAtlas.h"
#include "SpriteAtlas.h"
#include "string"

namespace AWayBack::UI::AnimationEditor
{
    class AnimatorController
    {
    public:
        uint8_t SelectedAnimationIndex;
        uint8_t BeginFrame, EndFrame;
        float FrameDuration;
        bool IsRepeatable;

        void CreateNewAnimationAtlas(const std::string& name, const std::string& folder, const SpriteAtlas& spriteAtlas);
        const Animation& CreateAnimation();
        [[nodiscard]] const std::vector<Animation>& GetAnimations() const;
        [[nodiscard]] const Animation& GetAnimation(uint8_t animationIndex) const;
        [[nodiscard]] const SpriteAtlas& GetSpriteAtlas() const;

    private:
        AnimationAtlas _animationAtlas;
    };
}