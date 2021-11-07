#include "AnimatorController.h"

#include <stdexcept>

namespace AWayBack::UI::AnimationEditor
{
    void AnimatorController::CreateNewAnimationAtlas(const std::string& name, const std::string& folder, const SpriteAtlas& spriteAtlas)
    {
        _animationAtlas = AnimationAtlas();
        _animationAtlas.Name = name;
        _animationAtlas.Folder = folder;
        _animationAtlas.SpriteAtlas = spriteAtlas;
    }

    const std::vector<Animation>& AnimatorController::GetAnimations() const
    {
        return _animationAtlas.Animations;
    }

    const Animation& AnimatorController::CreateAnimation()
    {
        _animationAtlas.Animations.emplace_back();

        return _animationAtlas.Animations.back();
    }

    const Animation& AnimatorController::GetAnimation(uint8_t animationIndex) const
    {
        if (animationIndex >= _animationAtlas.Animations.size())
            throw std::invalid_argument("animationIndex is out of range");

        return _animationAtlas.Animations[animationIndex];
    }

    const SpriteAtlas& AnimatorController::GetSpriteAtlas() const
    {
        return _animationAtlas.SpriteAtlas;
    }

}
