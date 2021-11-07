#include "NewSpriteAtlasModal.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "ImGuiExt.h"
#include "AWayBack/Utils/FileDialog.h"

#include <filesystem>

namespace AWayBack
{
    void NewSpriteAtlasModal::Open()
    {
        _isOpenRequested = true;
    }

    bool NewSpriteAtlas(const char* name, AWayBack::SpriteAtlasData& spriteAtlas, bool& isOpen)
    {
        if (!ImGui::BeginCenteredModal(name, &isOpen))
        {            
            return false;
        }
        
        ImGui::InputText("Name", &spriteAtlas.Name);
        ImGui::InputText("Folder", &spriteAtlas.Folder);

        if (ImGui::Button("..."))
        {
            std::optional<std::string> folderPath = FileDialog::OpenFolder();

            if (folderPath)
            {
                spriteAtlas.Folder = folderPath.value();
            }
        }

        ImGui::NewLine();

        ImGui::InputText("Texture", &spriteAtlas.TexturePath);

        ImGui::PushID("BrowseTexture");

        if (ImGui::Button("..."))
        {
            std::optional<std::string> texturePath = FileDialog::OpenFile("Textures (*.png)\0*.png\0");

            if (texturePath)
            {
                spriteAtlas.TexturePath = texturePath.value();
            }
        }

        ImGui::PopID();

        if (ImGui::Button("OK"))
        {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();

            return true;
        }

        ImGui::EndPopup();

        return false;
    }

    void NewSpriteAtlasModal::Render()
    {
        const char* popupName = "New sprite atlas";

        if (_isOpenRequested)
        {
            ImGui::OpenPopup(popupName);
            _spriteAtlasData = {};

            _isOpen = true;
            _isOpenRequested = false;
        }

        if (NewSpriteAtlas(popupName, _spriteAtlasData, _isOpen))
        {
            _controller.CreateNewSpriteAtlas(_spriteAtlasData);
        }
    }
}
