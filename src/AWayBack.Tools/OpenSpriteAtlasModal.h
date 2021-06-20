#pragma once
#include <string>
#include <filesystem>

namespace AWayBack
{
    class OpenSpriteAtlasModal
    {
    public:


        bool IsOpenRequested;
        bool IsOpen;

        std::string& GetSelectedFile() { return _selectedFile; }

        bool Render();
    private:
        const char* Name = "Open sprite sheet";
        std::string _selectedFile;
    };
}
