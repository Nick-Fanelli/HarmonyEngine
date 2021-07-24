#include "EditorScene.h"

#include <glm/gtc/type_ptr.hpp>

#include <Core/Input.h>

#include <Scene/Component.h>
#include <Scene/SceneSerialization.h>
#include <Scene/Entity.h>

#include <Layers/ImGuiLayer.h>
#include <Layers/RenderLayer.h>

#include "Application.h"
#include "EditorCamera.h"
#include "MenuBar.h"
#include "Settings.h"

#include "Panels/HierarchyEditorPanel.h"

using namespace HarmonyEditor;
using namespace HarmonyEngine;

static ImGuiLayer s_ImGuiLayer;
static RenderLayer s_RenderLayer;

static HierarchyEditorPanel s_HierarchyEditorPanel;

static MenuBar s_MenuBar;

static EditorCamera s_Camera;

static SceneSerializer s_SceneSerializer;

static ImGuizmo::OPERATION s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

static bool s_IsViewportSelected = false;
static bool s_IsTabToggled = false;

static std::vector<Setting<bool>*> s_TabPointers;

const ImGuizmo::OPERATION& EditorScene::GetCurrentOperation() { return s_CurrentImGuizmoOperation; }
void EditorScene::SetCurrentOperation(const ImGuizmo::OPERATION& operation) { s_CurrentImGuizmoOperation = operation; }

void EditorScene::OnCreate() {
    HARMONY_PROFILE_FUNCTION();

    SettingsManager::LoadSettings();

    s_ImGuiLayer = this;
    s_MenuBar = this;
    s_RenderLayer = { &s_Camera, &m_SelectedScene };
    s_HierarchyEditorPanel = this;

    std::string iniSaveLocation = std::filesystem::path(Application::GetApplicationSupportDirectory()) / "window-layout.ini";

    s_ImGuiLayer.OnCreate(iniSaveLocation);
    s_RenderLayer.OnCreate();
}

void EditorScene::OpenScene(const std::filesystem::path& filepath) {
    s_SceneSerializer = SceneSerializer(&m_SelectedScene, filepath);
    s_SceneSerializer.DeserializeYAML();
}

void EditorScene::SaveScene() {
    if(s_SceneSerializer)
        s_SceneSerializer.SerializeYAML();
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
    s_IsViewportSelected = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

    ImGui::BeginChild("Render"); {

        ImVec2 wsize = ImGui::GetWindowSize();
        s_Camera.SetViewportSize(wsize.x, wsize.y);

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

    if(Input::IsKeyDown(HARMONY_KEY_T))
        s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
    if(Input::IsKeyDown(HARMONY_KEY_R))
        s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::ROTATE;
    if(Input::IsKeyDown(HARMONY_KEY_S))
        s_CurrentImGuizmoOperation = ImGuizmo::OPERATION::SCALE;

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

    SettingsManager::OnImGuiRender();

    s_ImGuiLayer.End();

    // Render Layer
    s_RenderLayer.Render();
}   

void EditorScene::OnDestroy() {
    SaveScene();

    SettingsManager::SaveSettings();

    s_ImGuiLayer.OnDestroy();
    s_RenderLayer.OnDestroy();
}