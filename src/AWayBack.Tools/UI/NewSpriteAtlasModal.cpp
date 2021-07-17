#include "NewSpriteAtlasModal.h"
#include "imgui.h"
#include "ImGuiExt.h"

namespace AWayBack
{
    void NewSpriteAtlasModal::Open()
    {
        _isOpenRequested = true;
    }

    bool NewSpriteAtlas(const char* name, AWayBack::SpriteAtlasData& spriteAtlas, bool& isOpen)
    {
        namespace fs = std::filesystem;

        if (!ImGui::BeginCenteredModal(name, &isOpen))
        {
            
            return false;
        }

        char stringBuffer[_MAX_PATH] = { 0 };
        strncpy_s(stringBuffer, spriteAtlas.Name.c_str(), sizeof stringBuffer);
        if (ImGui::InputText("Name", stringBuffer, sizeof stringBuffer))
        {
            spriteAtlas.Name = std::string(stringBuffer);
        }

        memset(stringBuffer, 0, sizeof stringBuffer);
        strncpy_s(stringBuffer, spriteAtlas.Folder.c_str(), sizeof stringBuffer);
        if (ImGui::InputText("Folder", stringBuffer, sizeof stringBuffer))
        {
            spriteAtlas.Folder = std::string(stringBuffer);
        }

        if (ImGui::Button("..."))
        {
            std::optional<std::string> folderPath = AWayBack::FileDialog::OpenFolder();

            if (folderPath)
            {
                spriteAtlas.Folder = folderPath.value();
            }
        }

        ImGui::NewLine();

        memset(stringBuffer, 0, sizeof stringBuffer);
        strncpy_s(stringBuffer, spriteAtlas.TexturePath.c_str(), sizeof stringBuffer);
        if (ImGui::InputText("Texture", stringBuffer, sizeof stringBuffer))
        {
            spriteAtlas.TexturePath = std::string(stringBuffer);
        }

        ImGui::PushID("BrowseTexture");

        if (ImGui::Button("..."))
        {
            std::optional<std::string> texturePath = AWayBack::FileDialog::OpenFile("Textures (*.png)\0*.png\0");

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
