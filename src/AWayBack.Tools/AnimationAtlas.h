#pragma once

#include "SpriteAtlas.h"
#include "string"
#include "vector"

namespace AWayBack::UI::AnimationEditor
{
    struct Animation
    {
        std::string Name;
        int32_t BeginFrame;
        int32_t EndFrame;
        float FrameTime;
        bool IsRepeatable;
    };

    struct AnimationAtlas
    {
        AnimationAtlas(std::string folder, std::string name, std::vector<Animation> animations, SpriteAtlas* spriteAtlas);
        AnimationAtlas(const AnimationAtlas& animationAtlas) = delete;
        ~AnimationAtlas();

        std::string Folder;
        std::string Name;
        std::vector<Animation> Animations;
        SpriteAtlas* SpriteAtlas;
    };

    struct AnimationAtlasData
    {
        std::string Folder;
        std::string Name;
        std::string SpriteAtlas;
    };
}