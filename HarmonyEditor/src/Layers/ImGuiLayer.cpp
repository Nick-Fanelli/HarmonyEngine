#include "ImGuiLayer.h"

#include <glm/glm.hpp>

#include <Core/Display.h>

#include "../Theme.h"

#include "../EditorPanels/HierarchyEditorPanel.h"
#include "../EditorPanels/InspectorEditorPanel.h"

static EditorScene* s_EditorScenePtr;
static const char* s_SaveFilename = "window-layout.ini";

static HierarchyEditorPanel s_HierarchyEditorPanel;
static InspectorEditorPanel s_InspectorEditorPanel;

bool ImGuiLayer::GetIsEditorSelected() { return s_IsEditorSelected; }
Entity& ImGuiLayer::GetSelectedEntity() { return s_SelectedEntity; }

void ImGuiLayer::SetSelectedEntity(Entity& entity) { s_SelectedEntity = entity; }

void ImGuiLayer::OnCreate(EditorScene* editorScenePtr) {

    HARMONY_PROFILE_FUNCTION();

    s_EditorScenePtr = editorScenePtr;

    const char* glslVersion = "#version 410 core";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = s_SaveFilename;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImFontConfig config = ImFontConfig();
    config.OversampleH = 3;
    config.OversampleV = 3;

    io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/segoe-ui.ttf", 18.0f, &config);

    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    Display::GetImGuiSize(&io.DisplaySize); // Set the ImGui display size

    ImGui_ImplGlfw_InitForOpenGL(Display::GetWindowPtr(), true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    s_HierarchyEditorPanel.OnCreate(s_EditorScenePtr);
    s_InspectorEditorPanel.OnCreate(s_EditorScenePtr);
}

static void DrawDockspace() {

    HARMONY_PROFILE_FUNCTION();

    static bool dockingEnabled = true;
    static constexpr bool optFullscreen = true;
    static constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if(optFullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if(dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        windowFlags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Dockspace", &dockingEnabled, windowFlags);
    ImGui::PopStyleVar();

    if(optFullscreen)
        ImGui::PopStyleVar(2);

    // Draw Dockspace
    static ImGuiIO& io = ImGui::GetIO();
    if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspaceID = ImGui::GetID("Dockspace");
        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
    }

    ImGui::End();
}

static void StartFrame() {

    HARMONY_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

static void EndFrame() {

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

void ImGuiLayer::ShowGameViewport() {

    HARMONY_PROFILE_FUNCTION();

    ImGui::Begin("Scene Editor");
    s_IsEditorSelected = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

    {
        ImGui::BeginChild("GameRender");

        ImVec2 wsize = ImGui::GetWindowSize();

        static const float aspectRatio = Display::GetAspectRatio(); 
        wsize.y = (float) wsize.x / aspectRatio;

        ImGui::Image((void*)(intptr_t) *s_EditorScenePtr->GetRenderTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();
    }

    ImGui::End();
}

static bool s_ShowDemoWindow = true;

void ImGuiLayer::OnUpdate() {

    HARMONY_PROFILE_FUNCTION();

    StartFrame();

    DrawDockspace();

    s_HierarchyEditorPanel.OnUpdate();
    s_InspectorEditorPanel.OnUpdate();

    ShowGameViewport();

    ImGui::ShowDemoWindow(&s_ShowDemoWindow);

    EndFrame();
}

void ImGuiLayer::OnDestroy() {
    ImGui::SaveIniSettingsToDisk(s_SaveFilename);
}