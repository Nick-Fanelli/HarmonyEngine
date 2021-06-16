#pragma once

#include <harmonypch.h>
#include <imguipch.h>

using namespace HarmonyEngine;

namespace Theme {

    struct ThemeData {
        ImVec4 TextColor;

        ImVec4 BackgroundColor;
        ImVec4 ChildBackgroundColor;
        ImVec4 ChildBackgroundHoverColor;
        ImVec4 ChildBackgroundActiveColor;

        ImVec4 SeparatorColor;
        ImVec4 SeparatorHoverColor;

        ImVec4 AccentColor;
        ImVec4 AccentHoverColor;

        ThemeData(const ImVec4& textColor, const ImVec4& backgroundColor, const ImVec4& childBackgroundColor,
                    const ImVec4& childBackgroundHoverColor, const ImVec4& childBackgroundActiveColor,
                    const ImVec4& separatorColor, const ImVec4& separatorHoverColor, const ImVec4& accentColor,
                    const ImVec4& accentHoverColor) : TextColor(textColor), BackgroundColor(backgroundColor),
                                                    ChildBackgroundColor(childBackgroundColor),
                                                    ChildBackgroundHoverColor(childBackgroundHoverColor),
                                                    ChildBackgroundActiveColor(childBackgroundActiveColor),
                                                    SeparatorColor(separatorColor),
                                                    SeparatorHoverColor(separatorHoverColor),
                                                    AccentColor(accentColor), AccentHoverColor(accentHoverColor) {}

        ThemeData() = default;
        ThemeData(const ThemeData&) = default;
    };

    static const char* s_ThemeType[] {"Dark", "Light"};
    static int s_SelectedTheme = 0;

    ThemeData GetTheme(const char* themeType) {
        ThemeData themeData;

        if(std::string(themeType) == std::string("Light")) {
            themeData = ThemeData(
                    ImVec4(0, 0, 0, 1),
                    ImVec4(1 - 0.235, 1 - 0.247, 1 - 0.254, 1),
                    ImVec4(1 - 0.169, 1 - 0.169, 1 - 0.169, 1),
                    ImVec4(1 - 0.21, 1 - 0.21, 1 - 0.21, 1),
                    ImVec4(1 - 0.23, 1 - 0.23, 1 - 0.23, 1),
                    ImVec4(1 - 0.3, 1 - 0.3, 1 - 0.3, 1),
                    ImVec4(1 - 0.35, 1 - 0.35, 1 - 0.35, 1),
                    ImVec4(0.2705882353, 0.8196078431, 0.4156862745, 1),
                    ImVec4(0.4, 0.9490196078, 0.5450980392, 1)
            );
        } else {
            themeData = ThemeData(
                    ImVec4(1, 1, 1, 1),
                    ImVec4(0.235, 0.247, 0.254, 1),
                    ImVec4(0.169, 0.169, 0.169, 1),
                    ImVec4(0.21, 0.21, 0.21, 1),
                    ImVec4(0.23, 0.23, 0.23, 1),
                    ImVec4(0.3, 0.3, 0.3, 1),
                    ImVec4(0.35, 0.35, 0.35, 1),
                    ImVec4(0.2705882353, 0.8196078431, 0.4156862745, 1),
                    ImVec4(0.4, 0.9490196078, 0.5450980392, 1)
            );
        }

        return themeData;
    }

}
