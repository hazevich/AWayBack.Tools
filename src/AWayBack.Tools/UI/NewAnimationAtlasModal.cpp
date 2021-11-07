#include "NewAnimationAtlasModal.h"

#include "imgui.h"
#include "AnimationAtlas.h"
#include "AtlasControls.h"

namespace AWayBack::UI::AnimationEditor
{
    NewAnimationAtlasModal::NewAnimationAtlasModal(AnimatorController& animationController)
        : _isOpen(false),
          _isOpenRequested(false),
          _animatorController(animationController)
    {
        
    }
    
    void NewAnimationAtlasModal::Open()
    {
        _isOpenRequested = true;
    }

    bool NewAnimationAtlas(const char* name, bool& isOpen, AnimationAtlasData& animationAtlas)
    {
        auto content = [&animationAtlas]() -> void
        {
            AtlasPathControls("AnimationAtlasPath", animationAtlas.Name, animationAtlas.Folder);

            ImGui::NewLine();

            BrowseControls("Sprite Atlas", animationAtlas.SpriteAtlas, "Sprite atlas (*.atlas)\0*.atlas\0");
        };

        return NewAtlas("New animation atlas", isOpen, content);
    }

    void NewAnimationAtlasModal::Render()
    {
        const char* popupName = "New animation atlas";

        if (_isOpenRequested)
        {   
            ImGui::OpenPopup(popupName);
            _animationAtlasData = {};
            _isOpen = true;
            _isOpenRequested = false;
        }

        if (NewAnimationAtlas(popupName, _isOpen, _animationAtlasData))
        {
            _animatorController.CreateNewAnimationAtlas(_animationAtlasData);
        }
    }
}
