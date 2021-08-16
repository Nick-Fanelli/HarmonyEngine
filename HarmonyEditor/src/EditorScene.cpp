#include "EditorScene.h"

#include <glm/gtc/type_ptr.hpp>

#include <Core/Input.h>

#include <Scene/Component.h>
#include <Scene/SceneSerialization.h>
#include <Scene/Entity.h>
#include <Scene/LuaScript.h>

#include <Layers/ImGuiLayer.h>
#include <Layers/RenderLayer.h>

#include <Render/MasterRenderer.h>

#include "Application.h"
#include "EditorCamera.h"
#include "MenuBar.h"
#include "Settings.h"
#include "ImGuiDefaults.h"
#include "Theme.h"

#include "Panels/HierarchyEditorPanel.h"
#include "Panels/AssetsEditorPanel.h"

using namespace HarmonyEditor;
using namespace HarmonyEngine;

static ImGuiLayer s_ImGuiLayer;
static RenderLayer s_RenderLayer;

static HierarchyEditorPanel s_HierarchyEditorPanel;
static AssetsEditorPanel s_AssetsEditorPanel;
static MenuBar s_MenuBar;

static EditorCamera s_Camera;

static SceneSerializer s_SceneSerializer;

static ImGuizmo::OPERATION s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

static bool s_IsViewportSelected = false;
static bool s_IsTabToggled = true;

static EditorScene* s_This;

static std::vector<Setting<bool>*> s_TabPointers;

static LuaScript s_LuaScript;

const ImGuizmo::OPERATION& EditorScene::GetCurrentOperation() { return s_CurrentImGuizmoOperation; }
void EditorScene::SetCurrentOperation(const ImGuizmo::OPERATION& operation) { s_CurrentImGuizmoOperation = operation; }
EditorCamera& EditorScene::GetEditorCamera() { return s_Camera; }

void EditorScene::SetActiveProject(const Project& project) { 
    m_ActiveProject = project; 
    CacheManager::LastOpenProject = project.GetProjectDirectory().string();
    s_AssetsEditorPanel.SyncAssets();

    std::string title = std::string(Application::GetDisplayTitle() + " | ") + project.GetProjectDirectory().stem().string();
    glfwSetWindowTitle(Display::GetWindowPtr(), title.c_str());
}

void EditorScene::OnCreate() {
    HARMONY_PROFILE_FUNCTION();

    SettingsManager::LoadSettings();
    CacheManager::LoadCache();

    s_This = this;

    s_ImGuiLayer = this;
    s_MenuBar = this;
    s_RenderLayer = { &s_Camera, &m_SelectedScene };
    s_HierarchyEditorPanel = this;
    s_AssetsEditorPanel = this;

    std::string iniSaveLocation = std::filesystem::path(Application::GetApplicationSupportDirectory()) / "window-layout.ini";

    s_ImGuiLayer.OnCreate(iniSaveLocation);

    // Set Theme
    Theme::SetTheme(Settings::EditorTheme.CurrentValue);
    if(Settings::EditorTheme == 0) {
        MasterRenderer::SetClearColor({ 0.8f, 0.8f, 0.8f, 1.0f });
    }

    s_RenderLayer.OnCreate();
    MasterRenderer::SetUseFramebuffer(true);

    Renderer::SetScenePtr(&m_SelectedScene);

    if(FileUtils::FileExists(CacheManager::LastOpenProject)) {
        SetActiveProject({CacheManager::LastOpenProject});
    }

    s_LuaScript.OpenScript("/Users/nick227889/Dev/Example Project/Assets/Scripts/TestScript.lua");
    s_LuaScript.OnCreate();
}

void EditorScene::OpenScene(const std::filesystem::path& filepath) {

    s_HierarchyEditorPanel.m_SelectedEntity = {};  // Clear the selected entity! (Important because entity will soon no longer exist!)

    AssetManager::DestroyAll(); // TODO: Reuse any assets that are shared!

    s_SceneSerializer = SceneSerializer(&m_SelectedScene, filepath);
    s_SceneSerializer.DeserializeYAML(m_ActiveProject.GetProjectDirectory());

    AssetManager::CreateAll();
}

void EditorScene::SaveScene() {
    if(s_SceneSerializer)
        s_SceneSerializer.SerializeYAML(m_ActiveProject.GetProjectDirectory());
}

static void DrawDockspace() {
    HARMONY_PROFILE_FUNCTION();

    static constexpr bool optFullscreen = true;
    static constexpr ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_None;

    static bool dockingEnabled = true;

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

    if(dockingFlags * ImGuiDockNodeFlags_PassthruCentralNode)
        windowFlags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    ImGui::Begin("Dockspace", &dockingEnabled, windowFlags);
    ImGui::PopStyleVar(optFullscreen ? 3 : 1);

    // Draw Dockspace
    static ImGuiIO& io = ImGui::GetIO();
    if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspaceID = ImGui::GetID("Dockspace");
        ImGui::DockSpace(dockspaceID, { 0.0f, 0.0f }, dockingFlags);
    }

    ImGui::End();
}

static void DrawGameViewport() {
    HARMONY_PROFILE_FUNCTION();

    if(!Settings::ShowViewportPanel)
        return;

    ImGui::Begin("Game Viewport", &Settings::ShowViewportPanel.CurrentValue);

#if HARMONY_DEBUG

    if(!MasterRenderer::GetUseFramebuffer())
        s_IsViewportSelected = true;
    else
        s_IsViewportSelected = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
#else

    s_IsViewportSelected = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

#endif

    ImGui::BeginChild("Render"); {

    ImVec2 wsize = ImGui::GetWindowSize();

#if HARMONY_DEBUG
    if(MasterRenderer::GetUseFramebuffer())
        s_Camera.SetViewportSize(wsize.x, wsize.y);
    else
        s_Camera.SetViewportSize(Display::GetWidth(), Display::GetHeight());

#else
    s_Camera.SetViewportSize(wsize.x, wsize.y);
#endif

        ImGui::Image((void*)(intptr_t) *s_RenderLayer.GetRenderTexture(), wsize, { 0.0, 1.0 }, { 1.0, 0.0 });

        // ImGuizmo
        if(s_HierarchyEditorPanel.GetSelectedEntity() && s_HierarchyEditorPanel.GetSelectedEntity().ContainsComponent<TransformComponent>()) {

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, (float) ImGui::GetWindowWidth(), (float) ImGui::GetWindowHeight());

            const glm::mat4& cameraProjection = s_Camera.GetProjctionMatrix();
            glm::mat4 cameraView = s_Camera.GetViewMatrix();

            auto& transformComponent = s_HierarchyEditorPanel.GetSelectedEntity().GetComponent<TransformComponent>();

            glm::mat4 transform = transformComponent.Transform.GetTransformationMatrix();

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                s_CurrentImGuizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(transform));

            if(ImGuizmo::IsUsing()) {
                ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(transformComponent.Transform.Position),
                    glm::value_ptr(transformComponent.Transform.Rotation), glm::value_ptr(transformComponent.Transform.Scale));
            }

        }

    } ImGui::EndChild();

    if(ImGui::BeginDragDropTarget()) {

        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ImGuiDefaults::HarmonyScenePathDragDropID)) {
            auto path = *(const std::filesystem::path*) payload->Data;
            s_This->OpenScene(path);
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

void EditorScene::OnUpdate(float deltaTime) {

    HARMONY_PROFILE_FUNCTION();

    // Camera Update
    if(s_IsViewportSelected)
        s_Camera.OnUpdate(deltaTime);

    if(Input::IsKeyDown(HARMONY_KEY_TAB)) {

        if(s_IsTabToggled) {
            s_TabPointers.clear();

            for(auto& entry : Settings::AllShowPanelSettings) {
                if(entry.second == &Settings::ShowViewportPanel)
                    continue;

                if(entry.second->CurrentValue) {
                    s_TabPointers.push_back(entry.second);
                    entry.second->CurrentValue = false;
                }
            }
        } else {
            for(auto& ptr : s_TabPointers) {
                ptr->CurrentValue = true;
            }
        }

        s_IsTabToggled = !s_IsTabToggled;
    }

#if HARMONY_DEBUG   
    if(Input::IsKeyDown(HARMONY_KEY_F6)) {
        MasterRenderer::SetUseFramebuffer(!MasterRenderer::GetUseFramebuffer());
    }
#endif

    if(Input::IsKeyDown(HARMONY_KEY_T))
        s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
    if(Input::IsKeyDown(HARMONY_KEY_R))
        s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::ROTATE;
    if(Input::IsKeyDown(HARMONY_KEY_S))
        s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::SCALE;

    s_LuaScript.OnUpdate(deltaTime);

    // ImGui Layer
    s_ImGuiLayer.Begin();
    ImGuizmo::BeginFrame();

    s_MenuBar.OnImGuiRender();

    static ImGuiStyle& style = ImGui::GetStyle();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 370.0f, style.WindowMinSize.y }); // Set minimum window size for dockspace windows
    DrawDockspace(); // Draw the dockspace environment
    ImGui::PopStyleVar(); // Pop dockspace window min size

    // Game Viewport
    DrawGameViewport(); // Draw the game viewport

    s_HierarchyEditorPanel.OnImGuiRender();
    s_AssetsEditorPanel.OnImGuiRender();

    SettingsManager::OnImGuiRender();

    s_ImGuiLayer.End();

    // Render Layer
    s_RenderLayer.Begin();
    s_RenderLayer.Render();
    s_RenderLayer.End();
}

void EditorScene::OnDestroy() {
    s_LuaScript.OnDestroy();

    SaveScene();

    SettingsManager::SaveSettings();
    CacheManager::SaveCache();

    AssetManager::DestroyAll();

    s_ImGuiLayer.OnDestroy();
    s_RenderLayer.OnDestroy();
}