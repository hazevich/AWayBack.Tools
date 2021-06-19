#include "ToolsApplication.h"
#include "imgui.h"

namespace AWayBack
{
    ToolsApplication::~ToolsApplication()
    {
        delete _fatCatTexture;
    }

    void ToolsApplication::Initialize()
    {
        _fatCatTexture = Texture2D::FromFile("fatcat.png");
    }

    void ToolsApplication::Render()
    {
        if (ImGui::Begin("Test window"))
        {
            ImGui::Image((void*) (intptr_t) _fatCatTexture->GetTextureId(), ImVec2(_fatCatTexture->GetWidth(), _fatCatTexture->GetHeight()));
        }

        ImGui::End();    
    }

}