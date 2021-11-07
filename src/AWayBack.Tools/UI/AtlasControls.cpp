#include "AtlasControls.h"

#include <string>
#include <optional>

#include "imgui.h"
#include "ImGuiExt.h"
#include "misc/cpp/imgui_stdlib.h"
#include "AWayBack/Utils/FileDialog.h"

namespace AWayBack::UI
{
    void AtlasPathControls(const char* id, std::string& name, std::string& folder)
    {
        ImGui::InputText("Name", &name);
        ImGui::InputText("Folder", &folder);

        ImGui::PushID(id);

        if (ImGui::Button("..."))
        {
            std::optional<std::string> openedFolder = FileDialog::OpenFolder();

            if (openedFolder)
            {
                folder = openedFolder.value();
            }
        }

        ImGui::PopID();
    }

    void BrowseControls(const char* label, std::string& path, const char* fileFilter)
    {
        ImGui::InputText(label, &path);

        ImGui::PushID(label);

        if (ImGui::Button("..."))
        {
            std::optional<std::string> selectedPath = FileDialog::OpenFile(fileFilter);

            if (selectedPath)
            {
                path = selectedPath.value();
            }
        }

        ImGui::PopID();
    }

    bool NewAtlas(const char* name, bool& isOpen, const std::function<void()>& content)
    {
        if (!ImGui::BeginCenteredModal(name, &isOpen))
        {            
            return false;
        }
        
        content();

        if (ImGui::Button("OK"))
        {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();

            return true;
        }

        ImGui::EndPopup();

        return false;
    }

}
