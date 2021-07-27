#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor::Theme {

    enum ThemePreset {

        ThemePresetLight,
        ThemePresetDark

    };

    void SetTheme(ThemePreset theme);

}