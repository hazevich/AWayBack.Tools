#pragma once

#include "AWayBack/Graphics/GraphicsDevice.h"
#include "AWayBack/Core/Window.h"

namespace AWayBack
{
    class Application
    {
    public:
        virtual ~Application() = default;

        void Run();

        GraphicsDevice& GetGraphicsDevice() { return *_graphicsDevice; }        
    protected:
        virtual void Render() {}
        virtual void Initialize() {}

    private:
        void OnClose();
        void OnWindowResized(const WindowResizedData& data);

    private:
        bool _isRunning = true;
        GraphicsDevice* _graphicsDevice;
    };
}