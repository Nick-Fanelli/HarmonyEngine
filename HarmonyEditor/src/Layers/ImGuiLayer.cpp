#include "ImGuiLayer.h"

#include <glm/glm.hpp>

#include <Core/Display.h>
#include <Scene/Component.h>

#include "../Theme.h"

static EditorScene* s_EditorScenePtr;

static void ApplySelectedTheme(ImGuiStyle& style) {

    Theme::ThemeData themeData = Theme::GetTheme(Theme::s_ThemeType[Theme::s_SelectedTheme]);

    style.Colors[ImGuiCol_Text] = themeData.TextColor;
    style.Colors[ImGuiCol_TextSelectedBg] = themeData.AccentColor;

    float disableColor = Theme::s_SelectedTheme == 0 ? 0.6f : 0.4f;
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(disableColor, disableColor, disableColor, 1); // Todo: Become Dynamic

    style.Colors[ImGuiCol_WindowBg] = themeData.BackgroundColor;
    style.Colors[ImGuiCol_ChildBg] = themeData.BackgroundColor;
    style.Colors[ImGuiCol_PopupBg] = ImVec4(themeData.ChildBackgroundColor.x, themeData.ChildBackgroundColor.y, themeData.ChildBackgroundColor.z, 1);
    style.Colors[ImGuiCol_DockingPreview] = themeData.AccentColor;

    style.Colors[ImGuiCol_Separator] = themeData.SeparatorColor;
    style.Colors[ImGuiCol_SeparatorActive] = themeData.SeparatorHoverColor;
    style.Colors[ImGuiCol_SeparatorHovered] = themeData.SeparatorHoverColor;

    style.Colors[ImGuiCol_Border] = themeData.BackgroundColor;
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);

    style.Colors[ImGuiCol_FrameBg] = themeData.ChildBackgroundColor;
    style.Colors[ImGuiCol_FrameBgHovered] = themeData.ChildBackgroundHoverColor;
    style.Colors[ImGuiCol_FrameBgActive] = themeData.ChildBackgroundActiveColor;

    style.Colors[ImGuiCol_TitleBg] = themeData.ChildBackgroundColor;
    style.Colors[ImGuiCol_TitleBgActive] = themeData.ChildBackgroundColor;
    style.Colors[ImGuiCol_TitleBgCollapsed] = themeData.BackgroundColor;

    style.Colors[ImGuiCol_MenuBarBg] = themeData.ChildBackgroundColor;

    style.Colors[ImGuiCol_ScrollbarGrab] = themeData.BackgroundColor;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = themeData.BackgroundColor;
    style.Colors[ImGuiCol_ScrollbarBg] = themeData.ChildBackgroundColor;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = themeData.ChildBackgroundActiveColor;

    style.Colors[ImGuiCol_CheckMark] = themeData.AccentColor;

    style.Colors[ImGuiCol_SliderGrab] = themeData.BackgroundColor;
    style.Colors[ImGuiCol_SliderGrabActive] = themeData.ChildBackgroundHoverColor;

    style.Colors[ImGuiCol_Button] = themeData.ChildBackgroundColor;
    style.Colors[ImGuiCol_ButtonHovered] = themeData.ChildBackgroundHoverColor;
    style.Colors[ImGuiCol_ButtonActive] = themeData.ChildBackgroundActiveColor;

    style.Colors[ImGuiCol_Header] = themeData.ChildBackgroundColor;
    style.Colors[ImGuiCol_HeaderHovered] = themeData.ChildBackgroundHoverColor;
    style.Colors[ImGuiCol_HeaderActive] = themeData.ChildBackgroundActiveColor;

    style.Colors[ImGuiCol_ResizeGrip] = themeData.ChildBackgroundColor;
    style.Colors[ImGuiCol_ResizeGripHovered] = themeData.ChildBackgroundHoverColor;
    style.Colors[ImGuiCol_ResizeGripActive] = themeData.ChildBackgroundActiveColor;

    style.Colors[ImGuiCol_PlotLines] = themeData.AccentColor;
    style.Colors[ImGuiCol_PlotLinesHovered] = themeData.AccentHoverColor;
    style.Colors[ImGuiCol_PlotHistogram] = themeData.AccentColor;
    style.Colors[ImGuiCol_PlotHistogramHovered] = themeData.AccentHoverColor;

    style.Colors[ImGuiCol_Tab] = ImLerp(style.Colors[ImGuiCol_Header], style.Colors[ImGuiCol_TitleBgActive], 0.90f);
    style.Colors[ImGuiCol_TabHovered] = style.Colors[ImGuiCol_HeaderHovered];
    style.Colors[ImGuiCol_TabActive] = themeData.ChildBackgroundActiveColor;
    style.Colors[ImGuiCol_TabUnfocused] = ImLerp(style.Colors[ImGuiCol_Tab], style.Colors[ImGuiCol_TitleBg], 0.80f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImLerp(style.Colors[ImGuiCol_TabActive],
                                                        style.Colors[ImGuiCol_TitleBg], 0.40f);

}

void ImGuiLayer::OnCreate(EditorScene* editorScenePtr) {

    s_EditorScenePtr = editorScenePtr;

    const char* glslVersion = "#version 410 core";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ApplySelectedTheme(style);

    Display::GetImGuiSize(&io.DisplaySize); // Set the ImGui display size

    ImGui_ImplGlfw_InitForOpenGL(Display::GetWindowPtr(), true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}


static void StartFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

static void EndFrame() {
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

static void ShowHierarchy() {
    ImGui::Begin("Hierarchy");

    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

    if(ImGui::TreeNode(s_EditorScenePtr->GetSceneName().c_str())) {
        s_EditorScenePtr->GetRegistry().view<EntityTag>().each([&](auto& tag) {
            if(ImGui::TreeNode(tag.Name.c_str())) {
                ImGui::TreePop();
            }
        });
        
        ImGui::TreePop();
    }

    ImGui::End();
}

// static bool s_ShowDemoWindow = true;

void ImGuiLayer::OnUpdate() {
    StartFrame();

    ShowHierarchy();

    // ImGui::ShowDemoWindow(&s_ShowDemoWindow);

    EndFrame();
}

void ImGuiLayer::OnDestroy() {

}