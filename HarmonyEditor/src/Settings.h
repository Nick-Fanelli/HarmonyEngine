#pragma once

#include <harmonypch.h>

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

    class Settings {

        friend class SettingsManager;

    public:
        // Not Displayed
        static Setting<bool> ShowViewportPanel;
        static Setting<bool> ShowSettingsPanel;
        static Setting<bool> ShowHierarchyPanel;
        static Setting<bool> ShowComponentsPanel;

        // Displayed
        static Setting<float> EditorMovementSensitivity;
    };

}