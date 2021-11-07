#include "AnimationAtlas.h"

namespace AWayBack::UI::AnimationEditor
{
    AnimationAtlas::AnimationAtlas(std::string folder, std::string name, std::vector<Animation> animations, AWayBack::SpriteAtlas* spriteAtlas)
        : Folder(std::move(folder)), Name(std::move(name)), Animations(std::move(animations)), SpriteAtlas(spriteAtlas)
    {
        
    }

    AnimationAtlas::~AnimationAtlas()
    {
        delete SpriteAtlas;
    }
}
