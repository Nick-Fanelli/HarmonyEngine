#include "Theme.h"

using namespace HarmonyEditor;

const char* Theme::ThemePreset[]  = {
    "Light", "Dark"
};

const uint32_t Theme::ThemePresetCount = sizeof(Theme::ThemePreset) / sizeof(Theme::ThemePreset[0]);

static void ThemeLight() {
    auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg] = ImVec4{ 0.9f, 0.895f, 0.89f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.8f, 0.795f, 0.79f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.7f, 0.695f, 0.69f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.85f, 0.8495f, 0.8495f, 1.0f };
    
    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.8f, 0.795f, 0.79f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.7f, 0.695f, 0.69f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.85f, 0.8495f, 0.849f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.8f, 0.795f, 0.79f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.7f, 0.695f, 0.69f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.85f, 0.8495f, 0.849f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.85f, 0.8495f, 0.849f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.62f, 0.6195f, 0.619f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.72f, 0.7195f, 0.719f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.85f, 0.8495f, 0.849f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.8f, 0.795f, 0.79f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.85f, 0.8495f, 0.849f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.85f, 0.8495f, 0.849f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.85f, 0.8495f, 0.849f, 1.0f };

    colors[ImGuiCol_Text] = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
    colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.8f, 0.795f, 0.79f, 1.0f };
    colors[ImGuiCol_PopupBg] = ImVec4{ 0.8f, 0.795f, 0.79f, 1.0f };
}

static void ThemeDark() {
    auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    
    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}

void Theme::SetTheme(int theme) {
    switch(theme) {
        case 0:
            ThemeLight();
            break;
        case 1:
            ThemeDark();
            break;
        default:
            ThemeDark();
            break;
    }
}