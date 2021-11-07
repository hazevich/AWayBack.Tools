#include "AnimatorController.h"

namespace AWayBack::UI::AnimationEditor
{
    void AnimatorController::CreateNewAnimationAtlas(const AnimationAtlasData& animationAtlasData)
    {
        delete _animationAtlas;
        _animationAtlas = new AnimationAtlas(animationAtlasData.Folder, animationAtlasData.Name, std::vector<Animation>(), LoadSpriteAtlasFromFile(animationAtlasData.SpriteAtlas));
    }

    const std::vector<Animation>& AnimatorController::GetAnimations() const
    {
        return _animationAtlas->Animations;
    }

    const Animation& AnimatorController::CreateAnimation()
    {
        _animationAtlas->Animations.emplace_back();

        return _animationAtlas->Animations.back();
    }

    const Animation& AnimatorController::GetAnimation(uint8_t animationIndex) const
    {
        if (animationIndex >= _animationAtlas->Animations.size())
            throw std::invalid_argument("animationIndex is out of range");

        return _animationAtlas->Animations[animationIndex];
    }

    const AnimationAtlas* AnimatorController::GetAnimationAtlas() const
    {
        return _animationAtlas;
    }
}
