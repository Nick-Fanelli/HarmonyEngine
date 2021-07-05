#pragma once

#include <harmonypch.h>
#include <imguipch.h>

#include <filesystem>

#include "Layers/MenuBarLayer.h"

using namespace HarmonyEngine;

class Settings {

    friend class MenuBarLayer;

public:
    template<typename DataType>
    struct Setting {

        DataType DefaultValue;
        DataType CurrentValue;
        std::string SettingID;

        Setting(const DataType& dataType, const char* settingID) 
        : DefaultValue(dataType), CurrentValue(dataType), SettingID(settingID) {}

        operator DataType&() { return CurrentValue; }
        operator DataType() const { return CurrentValue; }

        const DataType& GetCurrentValue() const { return CurrentValue; }
        const DataType& GetDefaultValue() const { return DefaultValue; }
    };

private:
    // Cache
    static Setting<bool> s_ShouldCacheCurrentProject;
    static Setting<bool> s_ShouldCacheCurrentScene;

    // Panel
    static Setting<bool> s_ShouldShowRendererStats;
    static Setting<bool> s_ShouldShowEnvironmentSettings;
    static Setting<bool> s_ShouldShowAssetsStats;
    static Setting<bool> s_ShouldShowProjectInfo;
    static Setting<bool> s_ShouldShowGlobalSettings;

    // Assets
    static Setting<int> s_AssetsUpdateSeconds;

    static const std::filesystem::path& GetSaveFilepath();

public:
    // Cache
    static bool ShouldCacheCurrentProject() { return s_ShouldCacheCurrentProject; }
    static bool ShouldCacheCurrentScene() { return s_ShouldCacheCurrentScene; }

    // Panel
    static bool ShouldShowRendererStats() { return s_ShouldShowRendererStats; }
    static bool ShouldShowEnvironmentSettings() { return s_ShouldShowEnvironmentSettings; }
    static bool ShouldShowAssetsStats() { return s_ShouldShowAssetsStats; }
    static bool ShouldShowProjectInfo() { return s_ShouldShowProjectInfo; }
    static bool ShouldShowGlobalSettings() { return s_ShouldShowGlobalSettings; }

    // Assets
    static int GetAssetsUpdateSecond() { return s_AssetsUpdateSeconds; }

public:
    void LoadSettings();
    void SaveSettings();

    void LoadCacheData();
    void SaveCacheData();

    void OnImGuiRender();
};