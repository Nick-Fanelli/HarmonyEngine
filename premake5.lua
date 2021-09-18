workspace "HarmonyEngine"
    architecture "x86_64"
    startproject "HarmonyEditor"

    flags {
        "MultiProcessorCompile"
    }

    configurations {
        "Debug",
        "Release",
        "Dist"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "HarmonyEngine/premake5.lua"
include "HarmonyEditor/premake5.lua"