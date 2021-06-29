#include "AWayBack/Utils/FileDialog.h"

#include <Windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Application.h"


namespace AWayBack::FileDialog
{
    std::optional<std::string> OpenFile(const char* filter)
    {
        OPENFILENAMEA openFileName;
        CHAR fileNameBuffer[260] = { 0 };
        ZeroMemory(&openFileName, sizeof OPENFILENAMEA);
        openFileName.lStructSize = sizeof OPENFILENAMEA;
        openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*) Application::GetCurrent().GetWindow().GetNativeWindow());
        openFileName.lpstrFile = fileNameBuffer;
        openFileName.nMaxFile = sizeof fileNameBuffer;
        openFileName.lpstrFilter = filter;
        openFileName.nFilterIndex = 1;
        openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameA(&openFileName) == TRUE)
        {
            return openFileName.lpstrFile;
        }

        return std::nullopt;
    }
}
