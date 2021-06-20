#pragma once

#include "imgui.h"
#include "AWayBack/Graphics/Texture.h"

namespace ImGui
{
    static bool BeginCenteredModal(const char* name, ImGuiWindowFlags flags = ImGuiBackendFlags_None, bool isClosable = true)
    {
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(.5f, .5f));

        if (isClosable)
        {
            auto isOpen = true;

            return ImGui::BeginPopupModal(name, &isOpen, flags);
        }

        return ImGui::BeginPopupModal(name, nullptr, flags);
    }

    static void Image(AWayBack::Texture2D& texture)
    {
        ImGui::Image((void*) (intptr_t) texture.GetTextureId(), ImVec2(texture.GetWidth(), texture.GetHeight()));
    }
}