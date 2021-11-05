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
        void Close();

        GraphicsDevice& GetGraphicsDevice() { return *_graphicsDevice; }
        Window& GetWindow() { return *_window; }

        static Application& GetCurrent() { return *_current; }
    protected:
        virtual void Render() {}
        virtual void Initialize() {}

    private:
        void OnClose();
        void OnWindowResized(const WindowResizedData& data);

    private:
        static Application* _current;

        bool _isRunning = true;
        GraphicsDevice* _graphicsDevice = nullptr;
        Window* _window = nullptr;
    };
}