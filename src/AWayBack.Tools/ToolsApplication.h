#pragma once

#include "Application.h"
#include "AWayBack/Graphics/Texture.h";

namespace AWayBack
{
    class ToolsApplication : public Application
    {
    protected:
        ~ToolsApplication() override;

        void Render() override;
        void Initialize() override;

    private:
        Texture2D* _fatCatTexture;
    };
}