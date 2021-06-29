#pragma once

#include <string>
#include <optional>

namespace AWayBack::FileDialog
{
    std::optional<std::string> OpenFile(const char* filter);
    std::optional<std::string> OpenFolder();
}
