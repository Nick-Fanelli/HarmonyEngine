#include "EditorLayer.h"

#include <glm/gtc/type_ptr.hpp>

#include <Core/Input.h>

#include <Scene/Component.h>
#include <Scene/SceneSerialization.h>
#include <Scene/Entity.h>
#include <Scene/LuaScript.h>

#include <Layers/ImGuiLayer.h>
#include <Layers/RenderLayer.h>

#include <Render/MasterRenderer.h>
#include <Render/Renderer2D.h>

#include "HarmonyEditor.h"
#include "EditorCamera.h"
#include "MenuBar.h"
#include "Settings.h"
#include "ImGuiDefaults.h"
#include "Theme.h"

#include "Panels/HierarchyEditorPanel.h"
#include "Panels/AssetsEditorPanel.h"

#include "Windows/CommonWindows.h"

using namespace HarmonyEditor;

static ImGuiLayer s_ImGuiLayer;

static HierarchyEditorPanel s_HierarchyEditorPanel;
static AssetsEditorPanel s_AssetsEditorPanel;
static MenuBar s_MenuBar;

static EditorCamera s_Camera;

static SceneSerializer s_SceneSerializer;

static ImGuizmo::OPERATION s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

static bool s_IsViewportSelected = false;
static bool s_IsTabToggled = false;

static EditorLayer* s_This;

static std::vector<Setting<bool>*> s_TabPointers;

const ImGuizmo::OPERATION& EditorLayer::GetCurrentOperation() { return s_CurrentImGuizmoOperation; }
void EditorLayer::SetCurrentOperation(const ImGuizmo::OPERATION& operation) { s_CurrentImGuizmoOperation = operation; }
EditorCamera& EditorLayer::GetEditorCamera() { return s_Camera; }

void EditorLayer::SetActiveProject(const Project& project) { 
    m_ActiveProject = project; 
    CacheManager::LastOpenProject = project.GetProjectDirectory().string();
    s_AssetsEditorPanel.SyncAssets();

    std::string title = std::string(HarmonyEditorApplication::GetDisplayTitle() + " | ") + project.GetProjectDirectory().stem().string();
    glfwSetWindowTitle(Display::GetWindowPtr(), title.c_str());
}

void EditorLayer::OnCreate() {
    HARMONY_PROFILE_FUNCTION();

    SettingsManager::LoadSettings();
    CacheManager::LoadCache();

    s_This = this;

    s_MenuBar = this;
    s_HierarchyEditorPanel = this;
    s_AssetsEditorPanel = this;

    std::string iniSaveLocation = std::filesystem::path(HarmonyEditorApplication::GetApplicationSupportDirectory()) / "window-layout.ini";

    s_ImGuiLayer = ImGuiLayer(m_EditorScenePtr);
    s_ImGuiLayer.OnCreate(iniSaveLocation);

    // Set Theme
    Theme::SetTheme(Settings::EditorTheme.CurrentValue);
    if(Settings::EditorTheme == 0) {
        MasterRenderer::SetClearColor({ 0.8f, 0.8f, 0.8f, 1.0f });
    }

    MasterRenderer::SetUseFramebuffer(true);
    MasterRenderer::CameraPtr = &s_Camera;

    if(FileUtils::FileExists(CacheManager::LastOpenProject)) {
        SetActiveProject({CacheManager::LastOpenProject});
    }
}

void EditorLayer::OpenScene(const std::filesystem::path& filepath) {

    // Save and reset the selected scene
    SaveScene();
    // *m_EditorScenePtr = {};

    s_HierarchyEditorPanel.m_SelectedEntity = {};  // Clear the selected entity! (Important because entity will soon no longer exist!)

    AssetManager::DestroyAll(); // TODO: Reuse any assets that are shared!

    s_SceneSerializer = SceneSerializer(m_EditorScenePtr, filepath);
    s_SceneSerializer.DeserializeYAML(m_ActiveProject.GetProjectDirectory());

    AssetManager::CreateAll();
}

void EditorLayer::SaveScene() {
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

void EditorLayer::DrawGameViewport() {
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
        MasterRenderer::CameraPtr->SetAspectRatio(wsize.x / wsize.y);
    else
        MasterRenderer::CameraPtr->SetAspectRatio(Display::GetWidth() / Display::GetHeight());

#else
    MasterRenderer::CameraPtr->SetAspectRatio(wsize.x / wsize.y);
#endif

        ImGui::Image((void*)(intptr_t) *m_EditorScenePtr->GetRenderLayer().GetRenderTexture(), wsize, { 0.0, 1.0 }, { 1.0, 0.0 });

        // ImGuizmo
        if(s_HierarchyEditorPanel.GetSelectedEntity() && s_HierarchyEditorPanel.GetSelectedEntity().ContainsComponent<TransformComponent>() && !m_IsRunning) {

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, (float) ImGui::GetWindowWidth(), (float) ImGui::GetWindowHeight());

            const glm::mat4& cameraProjection = s_Camera.GetProjectionMatrix();
            const glm::mat4& cameraView = s_Camera.GetViewMatrix();

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

void EditorLayer::StartRuntime() {
    if(s_SceneSerializer.IsAssigned()) {
        SaveScene();

        m_EditorScenePtr->ForEachEntityWithTransform<OrthographicCameraComponent>([&](TransformComponent& transform, OrthographicCameraComponent& component) {
            component.Camera.SetPosition(transform.Transform.Position);
            MasterRenderer::CameraPtr = &component.Camera;
            return;
        });

        m_EditorScenePtr->GetGlobalScript().Reload();
        m_EditorScenePtr->GetGlobalScript().OnCreate();
        m_IsRunning = true;
    }
}

void EditorLayer::StopRuntime() {
    m_IsRunning = false;
    
    if(s_SceneSerializer.IsAssigned()) {
        MasterRenderer::CameraPtr = &s_Camera;
        m_EditorScenePtr->GetGlobalScript().OnCreate();
    }
}

void EditorLayer::OnUpdate(float deltaTime) {

    HARMONY_PROFILE_FUNCTION();

    // Camera Update
    if((s_IsViewportSelected || s_Camera.IsInOperation()) && !m_IsRunning)
        s_Camera.OnUpdate(deltaTime);

    if(Input::IsKeyDown(HARMONY_KEY_TAB)) {

        if(s_IsTabToggled) {
            for(auto& ptr : s_TabPointers) {
                ptr->CurrentValue = true;
            }
        } else {
            s_TabPointers.clear();

            for(auto& entry : Settings::AllShowPanelSettings) {
                if(entry.second == &Settings::ShowViewportPanel)
                    continue;

                if(entry.second->CurrentValue) {
                    s_TabPointers.push_back(entry.second);
                    entry.second->CurrentValue = false;
                }
            }
            
        }

        s_IsTabToggled = !s_IsTabToggled;
    }

#if HARMONY_DEBUG   
    if(Input::IsKeyDown(HARMONY_KEY_F6)) {
        MasterRenderer::SetUseFramebuffer(!MasterRenderer::GetUseFramebuffer());
    }
#endif

    if(!ImGuiDefaults::IsInputFocused()) {
        if(Input::IsKeyDown(HARMONY_KEY_T))
            s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
        if(Input::IsKeyDown(HARMONY_KEY_R))
            s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::ROTATE;
        if(Input::IsKeyDown(HARMONY_KEY_S))
            s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::SCALE;
    }

    ImGuiDefaults::ResetIsInputFocused();

    // Runtime Updates
    if(m_IsRunning) {
        m_EditorScenePtr->GetGlobalScript().OnUpdate(deltaTime);
    }

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

    CommonWindows::OnImGuiRender();

    if(Settings::ShowRendererStats)
        RendererStats::DrawImGUIStats(&Settings::ShowRendererStats.CurrentValue);

    s_ImGuiLayer.End();
}

void EditorLayer::OnDestroy() {

    // Un-toggle tab
    if(s_IsTabToggled) {
        for(auto& ptr : s_TabPointers) {
            ptr->CurrentValue = true;
        }
    }

    SaveScene();

    SettingsManager::SaveSettings();
    CacheManager::SaveCache();

    AssetManager::DestroyAll();

    s_ImGuiLayer.OnDestroy();
}