#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

class MenuBarLayer {

public:

    static void OnCreate();
    static void OnUpdate();

    static bool ShouldShowRendererStats() { return s_ShowRendererStats; }
    static bool ShouldShowEnvironmentSettings() { return s_ShowEnvironmentSettings; }

private:
    static bool s_ShowRendererStats;
    static bool s_ShowEnvironmentSettings;

};