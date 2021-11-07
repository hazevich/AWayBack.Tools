#pragma once
#include <string>
#include <functional>

namespace AWayBack::UI
{
    void AtlasPathControls(const char* id, std::string& name, std::string& folder);

    void BrowseControls(const char* label, std::string& path, const char* fileFilter);

    bool NewAtlas(const char* name, bool& isOpen, const std::function<void()>& content);
}