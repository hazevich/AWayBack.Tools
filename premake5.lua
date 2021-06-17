workspace "AWayBack.Tools"
    configurations { "Debug", "Release" }
    startproject "AWayBack.Tools"
    architecture "x64"

targetdirPath = "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}/%{prj.name}"
objdirPath = "%{wks.location}/obj/%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}/%{prj.name}"

group "src"
    project "AWayBack.Core"
        kind "StaticLib"
        language "C++"
        targetdir (targetdirPath)
        objdir (objdirPath)
        location "src/AWayBack.Core"

        files { "src/AWayBack.Core/**.h", "src/AWayBack.Core/**.cpp" }

        links 
        { 
            "glfw",
            "glad",
            "imgui"
        }

        includedirs 
        { 
            "dependencies/glfw/include",
            "dependencies/glad/include",
            "dependencies/imgui",
            "src/AWayBack.Core"
        }

        filter "configurations:Debug"
            defines { "DEBUG" }
            runtime "Debug"
            symbols "On"
        
        filter "configurations:Release"
            defines { "RELEASE" }
            runtime "Release"
            optimize "On"

        filter "system:windows"
            defines { "WINDOWS" }

    project "AWayBack.Tools"
        kind "ConsoleApp"
        language "C++"
        targetdir (targetdirPath)
        objdir (objdirPath)
        location "src/AWayBack.Tools"

        files { "src/AWayBack.Tools/**.h", "src/AWayBack.Tools/**.cpp" } 

        includedirs { "%{wks.location}/src/AWayBack.Core" }

        links { "AWayBack.Core" }
        
        filter "configurations:Debug"
            defines { "DEBUG" }
            runtime "Debug"
            symbols "On"
        
        filter "configurations:Release"
            defines { "RELEASE" }
            runtime "Release"
            optimize "On"

group "dependencies"
    project "glfw"
        kind "StaticLib"
        language "C"
        targetdir (targetdirPath)
        objdir (objdirPath)
        location "dependencies/glfw"

        files
        {
            "dependencies/glfw/src/internal.h",
            "dependencies/glfw/src/mappings.h",
            "dependencies/glfw/src/context.c",
            "dependencies/glfw/src/init.c",
            "dependencies/glfw/src/input.c",
            "dependencies/glfw/src/monitor.c",
            "dependencies/glfw/src/vulkan.c",
            "dependencies/glfw/src/window.c",
            "dependencies/glfw/include/**.h"
        }

        filter "system:windows"
            defines 
            { 
                "_GLFW_WIN32",
                "_CRT_SECURE_NO_WARNINGS"
            }

            files
            {

                "dependencies/glfw/src/win32_platform.h",
                "dependencies/glfw/src/win32_joystick.h",
                "dependencies/glfw/src/win32_init.c",
                "dependencies/glfw/src/win32_joystick.c",
                "dependencies/glfw/src/win32_monitor.c",
                "dependencies/glfw/src/win32_time.c",
                "dependencies/glfw/src/win32_thread.c",
                "dependencies/glfw/src/win32_window.c",
                "dependencies/glfw/src/wgl_context.h",
                "dependencies/glfw/src/egl_context.h",
                "dependencies/glfw/src/osmesa_context.h",
                "dependencies/glfw/src/wgl_context.c",
                "dependencies/glfw/src/egl_context.c",
                "dependencies/glfw/src/osmesa_context.c"
            }

    project "glad"
        kind "StaticLib"
        language "C"
        targetdir (targetdirPath)
        objdir (objdirPath)
        location "dependencies/glad"

        files 
        {
            "dependencies/glad/**.h",
            "dependencies/glad/**.c"
        }

        includedirs { "dependencies/glad/include" }

    project "imgui"
        kind "StaticLib"
        language "C++"
        targetdir (targetdirPath)
        objdir (objdirPath)
        location "dependencies/imgui"

        files 
        {
            "dependencies/imgui/imconfig.h",
            "dependencies/imgui/imgui.h",
            "dependencies/imgui/imgui.cpp",
            "dependencies/imgui/imgui_demo.cpp",
            "dependencies/imgui/imgui_draw.cpp",
            "dependencies/imgui/imgui_internal.h",
            "dependencies/imgui/imgui_tables.cpp",
            "dependencies/imgui/imgui_widgets.cpp",
            "dependencies/imgui/imstb_rectpack.h",
            "dependencies/imgui/imstb_textedit.h",
            "dependencies/imgui/imstb_truetype.h",
            "dependencies/imgui/backends/imgui_impl_glfw.h",
            "dependencies/imgui/backends/imgui_impl_glfw.cpp",
            "dependencies/imgui/backends/imgui_impl_opengl3.h",
            "dependencies/imgui/backends/imgui_impl_opengl3.cpp",

        }

        links
        {
            "glad",
            "glfw"
        }

        includedirs
        {
            "dependencies/imgui",
            "dependencies/glfw/include",
            "dependencies/glad/include"
        }

        defines	{ "IMGUI_IMPL_OPENGL_LOADER_GLAD" }