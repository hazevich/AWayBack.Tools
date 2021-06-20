#include "SpriteEditor.h"
#include "imgui.h"
#include "ImGuiExt.h"

namespace AWayBack
{
    void SpriteEditor::SetTexture(Texture2D* texture)
    {
        _texture = texture;
        _spriteAtlas = { _texture->GetName(), _texture->GetName(), std::vector<Sprite>{} };
    }

    void SpriteEditor::Render()
    {
        RenderCanvas();
        RenderControls();
    }

    void SpriteEditor::RenderCanvas()
    {
        if (!ImGui::Begin("Canvas")) return;

        ImGui::BeginChild("CanvasFrame",
                          ImVec2(0, ImGui::GetContentRegionAvail().y),
                          true,
                          ImGuiWindowFlags_HorizontalScrollbar
                          | ImGuiWindowFlags_AlwaysAutoResize
        );

        if (_texture)
        {
            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
            ImVec2 cursorPos = ImGui::GetCursorPos();
            auto textureWidth = (float) _texture->GetWidth();
            auto textureHeight = (float) _texture->GetHeight();

            auto newCursorPos = ImVec2(
                contentRegionAvail.x > textureHeight
                    ? (contentRegionAvail.x - textureWidth) * 0.5f
                    : cursorPos.x,
                contentRegionAvail.y > textureWidth
                    ? (contentRegionAvail.y - textureHeight) * 0.5f
                    : cursorPos.x
            );

            ImGui::SetCursorPos(newCursorPos);

            ImGui::Image(*_texture);
        }

        ImGui::EndChild();

        ImGui::End();
    }

    void SpriteEditor::RenderControls()
    {
        if (!ImGui::Begin("Controls")) return;

        RenderSpriteAtlasHeader();

        ImGui::End();
    }

    void SpriteEditor::RenderSpriteAtlasHeader()
    {
        if (!ImGui::CollapsingHeader("Sprite atlas")) return;

        ImGui::LabelText("Sprite Atlas Name", _spriteAtlas.Name.c_str());
        ImGui::NewLine();
        ImGui::LabelText("Texture name", _spriteAtlas.TextureName.c_str());
    }

}
