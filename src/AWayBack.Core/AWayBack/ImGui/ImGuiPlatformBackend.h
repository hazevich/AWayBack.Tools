#pragma once

namespace AWayBack
{
    class ImGuiPlatformBackend
    {
    public:
        virtual void Initialize() = 0;
        virtual void NewFrame() = 0;
        virtual void RenderDrawData() = 0;

        static ImGuiPlatformBackend* Create(void* window);
    };
}