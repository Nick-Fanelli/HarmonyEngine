#pragma once

#include <harmonypch.h>
#include <imguipch.h>

#include <filesystem>

using namespace HarmonyEngine;

class Settings {

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
    // General
    static Setting<bool> s_ShouldCacheCurrentProject;

    // Assets
    static Setting<int> s_AssetsUpdateSeconds;

    static const std::filesystem::path& GetSaveFilepath();

public:
    // General
    static bool ShouldCacheCurrentProject() { return s_ShouldCacheCurrentProject; }

    // Assets
    static int GetAssetsUpdateSecond() { return s_AssetsUpdateSeconds; }

public:
    void LoadSettings();
    void SaveSettings();

    void LoadCacheData();
    void SaveCacheData();

    void OnImGuiRender();
};