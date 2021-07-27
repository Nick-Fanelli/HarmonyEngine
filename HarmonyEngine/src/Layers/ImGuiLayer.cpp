#include "ImGuiLayer.h"

#include "../Core/Display.h"

using namespace HarmonyEngine;

static std::string s_SaveFileLocation;

void ImGuiLayer::OnCreate(const std::string& saveFilelocation) {
    HARMONY_PROFILE_FUNCTION();

    s_SaveFileLocation = saveFilelocation;

    const char* glslVersion = "#version 410 core";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = saveFilelocation.c_str();
    ImGui::LoadIniSettingsFromDisk(s_SaveFileLocation.c_str());

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 3;

    // Default Font
    io.FontDefault = io.Fonts->AddFontFromFileTTF("engineAssets/fonts/segoe-ui.ttf", 18.0f, &config);

    // Font Awesome
    ImFontConfig faConfig;
    faConfig.MergeMode = true;
    faConfig.GlyphMinAdvanceX = 13.0f;
    static constexpr ImWchar iconRanges[] = { 0xe005, 0xf8ff, 0 };
    io.Fonts->AddFontFromFileTTF("engineAssets/fonts/font-awesome/Font-Awesome-Solid-900.otf", 16.0f, &faConfig, iconRanges);

    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    Display::GetImGuiSize(&io.DisplaySize); // Set ImGui display size
    
    ImGui_ImplGlfw_InitForOpenGL(Display::GetWindowPtr(), true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

void ImGuiLayer::Begin() {
    HARMONY_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End() {
    HARMONY_PROFILE_FUNCTION();

    static ImGuiIO& io = ImGui::GetIO();
    Display::GetImGuiSize(&io.DisplaySize);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* previousContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(previousContext);
    }
}

void ImGuiLayer::OnDestroy() {
    HARMONY_PROFILE_FUNCTION();

    ImGui::SaveIniSettingsToDisk(s_SaveFileLocation.c_str());
}