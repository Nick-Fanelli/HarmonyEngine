workspace "HarmonyEngine"
    architecture "x86_64"
    startproject "HarmonyEditor"

    configurations {
        "Debug",
        "Release"
    }

    flags {
        -- "MultiProcessorCompile"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "HarmonyEngine/premake5.lua"
include "HarmonyEditor/premake5.lua"