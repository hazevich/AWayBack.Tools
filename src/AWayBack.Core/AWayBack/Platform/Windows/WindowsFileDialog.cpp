#include "AWayBack/Utils/FileDialog.h"

#include <Windows.h>
#include <commdlg.h>
#include <ShObjIdl.h>
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

    std::optional<std::string> OpenFolder()
    {
        IFileDialog* fileDialog = nullptr;

        if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fileDialog))))
        {
            DWORD options;

            if (SUCCEEDED(fileDialog->GetOptions(&options)))
            {
                fileDialog->SetOptions(options | FOS_PICKFOLDERS);

                if (SUCCEEDED(fileDialog->Show(glfwGetWin32Window((GLFWwindow*) Application::GetCurrent().GetWindow().GetNativeWindow()))))
                {
                    IShellItem* result;

                    if (SUCCEEDED(fileDialog->GetResult(&result)))
                    {
                        PWSTR pwstrFilePath = nullptr;

                        if (SUCCEEDED(result->GetDisplayName(SIGDN_FILESYSPATH, &pwstrFilePath)))
                        {
                            char str[MAX_PATH];
                            size_t i;
                            wcstombs_s(&i, str, pwstrFilePath, MAX_PATH);

                            CoTaskMemFree(pwstrFilePath);

                            return str;
                        }

                        result->Release();
                    }
                }
            }

            fileDialog->Release();
        }

        return std::nullopt;
    }
}
