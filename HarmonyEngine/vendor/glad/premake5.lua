project "GLAD"
    kind "StaticLib"
    language "C"
    architecture "x86_64"

    targetdir ( "%{wks.location}/build/" .. outputdir .. "/%{prj.name}" )
    objdir ( "%{wks.location}/build-int/" .. outputdir .. "/%{prj.name}")

    includedirs { "include/" }

    files { 
        "glad.h",
        "khrplatform.h",
        "glad.c"
    }

    filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		defines
		{
			"_GLAD_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 
			"_GLAD_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
