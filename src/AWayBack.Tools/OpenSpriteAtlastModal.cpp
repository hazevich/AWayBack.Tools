#include "imgui.h"
#include "OpenSpriteAtlasModal.h"
#include "ImGuiExt.h"
#include "filesystem"

namespace fs = std::filesystem;

namespace AWayBack
{
    bool OpenSpriteAtlasModal::Render()
    {
        if (IsOpenRequested)
        {
            ImGui::OpenPopup(Name);
            IsOpenRequested = false;
            IsOpen = true;
        }
        else IsOpen = false;

        if (!ImGui::BeginCenteredModal(Name))
        {
            return false;
        }

        ImGui::BeginChild("SpriteAtlases", ImVec2(500, 230), true);

        fs::path currentDirectory = fs::current_path();

        for (const fs::directory_entry& entry : fs::directory_iterator(currentDirectory))
        {
            if (entry.path().extension() == ".png")
            {
                fs::path p = entry.path();
                if (ImGui::Selectable(p.filename().string().c_str(), p == _selectedFile))
                {
                    _selectedFile = p.string();
                }
            }
        }

        ImGui::EndChild();

        if (ImGui::Button("Ok"))
        {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return true;
        }

        ImGui::EndPopup();

        return false;
    }
}
