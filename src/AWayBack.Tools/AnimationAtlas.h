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
        std::string Folder;
        std::string Name;
        SpriteAtlas SpriteAtlas;
        std::vector<Animation> Animations;
    };
}