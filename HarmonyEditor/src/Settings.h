#pragma once

#include <harmonypch.h>

#include "EditorCamera.h"

using namespace HarmonyEngine;

namespace HarmonyEditor {

    template<typename T>
    struct Setting {

        const T DefaultValue;
        mutable T CurrentValue;

        Setting() = default;
        Setting(const T& defaultValue) : DefaultValue(defaultValue), CurrentValue(defaultValue) {}

        void Reset() const { CurrentValue = DefaultValue; }

        operator T&() const { return CurrentValue; }
    };

    class SettingsManager {

    public:
        static void LoadSettings();
        static void SaveSettings();
        
        static void OnImGuiRender();

    }; 

    class CacheManager {

    public: 
        static std::string LastOpenProject;

        static void LoadCache();
        static void SaveCache();

    };

    class Settings {

        friend class SettingsManager;

    public:
        // Not Displayed
        static Setting<bool> ShowViewportPanel;
        static Setting<bool> ShowSettingsPanel;
        static Setting<bool> ShowHierarchyPanel;
        static Setting<bool> ShowComponentsPanel;
        static Setting<bool> ShowAssetsPanel;

        static std::unordered_map<std::string, Setting<bool>*> AllShowPanelSettings;

        // Displayed
        static Setting<int> EditorInputStyle;
        static Setting<float> EditorMovementSensitivity;
    };

}