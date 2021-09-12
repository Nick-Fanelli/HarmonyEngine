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
        static inline Setting<bool> ShowViewportPanel = true;
        static inline Setting<bool> ShowSettingsPanel = true;
        static inline Setting<bool> ShowHierarchyPanel = true;
        static inline Setting<bool> ShowComponentsPanel = true;
        static inline Setting<bool> ShowAssetsPanel = true;
        static inline Setting<bool> ShowRendererStats = false;

        static std::unordered_map<std::string, Setting<bool>*> AllShowPanelSettings;

        // General
        static inline Setting<bool> VSyncEnabled = true;

        // General -> Viewport Settings
        static inline Setting<int> EditorInputStyle = EditorCamera::InputStyle::InputStyleDefault;
        static inline Setting<float> EditorMovementSensitivity = 3.0f;

        // General -> Assets Panel Settings
        static inline Setting<float> AssetsEditorPanelUpdateIntervalSeconds = 1.0f;

        // Appearance
        static inline Setting<int> EditorTheme = 1; // Dark
    };

}