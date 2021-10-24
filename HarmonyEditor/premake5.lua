project "HarmonyEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ( "%{wks.location}/build/" .. outputdir .. "/%{prj.name}")
    objdir ( "%{wks.location}/build-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs {
        "src",
        "vendor",
        "vendor/ImGuizmo",
        "vendor/nativefiledialog/src/include",

        "%{wks.location}/HarmonyEngine/src/include",
        "%{wks.location}/HarmonyEngine/vendor",
        "%{wks.location}/HarmonyEngine/vendor/glfw/include",
        "%{wks.location}/HarmonyEngine/vendor/lua",
        "%{wks.location}/HarmonyEngine/vendor/imgui",
        "%{wks.location}/HarmonyEngine/vendor/imgui/backends",
        "%{wks.location}/HarmonyEngine/vendor/yaml-cpp/include",
    }

    links {
        "HarmonyEngine",
        "GLFW",
        "GLAD",
        "lua",
        "ImGui",
        "yaml-cpp",
        "ImGuizmo",
        "NFD"
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

	filter "configurations:Release"
		runtime "Release"
        optimize "On"

        defines {
            "NDEBUG"
        }

include "vendor/ImGuizmo/premake5.lua"
include "vendor/nativefiledialog/premake5.lua"