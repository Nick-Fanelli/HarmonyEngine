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
        static Setting<float> EditorMovementSensitivity;
    };

}