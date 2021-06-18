#pragma once

namespace AWayBack
{
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;
        virtual void SwapBuffers() = 0;
    };
}