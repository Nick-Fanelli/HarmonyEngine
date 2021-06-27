#pragma once

#include <harmonypch.h>
#include <imguipch.h>

using namespace HarmonyEngine;

class Settings {

private: // Settings

    // Assets Settings
    static int s_AssetsUpdateSeconds;

public: // Getters for Settings

    // Assets Settings
    static int GetAssetsUpdateSeconds() { return s_AssetsUpdateSeconds; }

public:
    static void LoadSettings();
    static void SaveSettings();

    static void OnImGuiRender();

};