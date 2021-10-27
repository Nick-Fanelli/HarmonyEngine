project "HarmonyEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    pchheader "src/harmonypch.h"

    targetdir ( "%{wks.location}/build/" .. outputdir .. "/%{prj.name}")
    objdir ( "%{wks.location}/build-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "vendor",
        "vendor/glfw/include",
        "vendor/lua",
        "vendor/imgui",
        "vendor/imgui/backends",
        "vendor/yaml-cpp/include",

        "src"
    }

    links {
        "GLFW",
        "GLAD",
        "lua",
        "ImGui",
        "yaml-cpp"
    }

    filter "system:macosx"
        links {
            "Cocoa.framework",
            "OpenGL.framework",
            "IOKit.framework",
            "CoreVideo.framework"
        }

    filter "system:linux"
        links {
            "dl",
            "pthread"
        }

        defines {
            "_X11"
        }

    filter "system:windows"
        defines {
            "_WINDOWS"
        }

    filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

        defines {
            "DEBUG"
        }

        files {
            "debug/**.h",
            "debug/**.cpp"
        }

	filter "configurations:Release"
		runtime "Release"
        optimize "On"

        defines {
            "NDEBUG"
        }

include "vendor/glfw/premake5.lua"
include "vendor/glad/premake5.lua"
include "vendor/lua/premake5.lua"
include "vendor/imgui/premake5.lua"
include "vendor/yaml-cpp/premake5.lua"