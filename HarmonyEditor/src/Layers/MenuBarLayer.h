#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

class MenuBarLayer {

public:

    static void OnCreate();
    static void OnUpdate();

    static bool ShouldShowRendererStats() { return s_ShowRendererStats; }
    static bool ShouldShowEnvironmentSettings() { return s_ShowEnvironmentSettings; }
    static bool ShouldShowAssetsStats() { return s_ShowAssetsStats; }
    static bool ShouldShowProjectInfo() { return s_ShowProjectInfo; }
    static bool ShouldShowGlobalSettings() { return s_ShowGlobalSettings; }

private:
    static bool s_ShowRendererStats;
    static bool s_ShowEnvironmentSettings;
    static bool s_ShowAssetsStats;
    static bool s_ShowProjectInfo;
    static bool s_ShowGlobalSettings;

};