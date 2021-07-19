#include "EditorScene.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include <Scene/Component.h>

#include <Layers/ImGuiLayer.h>
#include <Layers/RenderLayer.h>

#include <Scene/Entity.h>

#include "EditorCamera.h"

#include "Panels/HierarchyEditorPanel.h"

#include "Application.h"

using namespace HarmonyEditor;
using namespace HarmonyEngine;

static ImGuiLayer s_ImGuiLayer;
static RenderLayer s_RenderLayer;

static HierarchyEditorPanel s_HierarchyEditorPanel;

static EditorCamera s_Camera{};

static bool s_IsViewportSelected = false;

void EditorScene::OnCreate() {
    HARMONY_PROFILE_FUNCTION();

    s_ImGuiLayer = this;
    s_RenderLayer = { &s_Camera, &m_SelectedScene };
    s_HierarchyEditorPanel = this;

    std::string iniSaveLocation = Application::GetApplicationSupportDirectory() + "/window-layout.ini";

    s_ImGuiLayer.OnCreate(iniSaveLocation);
    s_RenderLayer.OnCreate();

    Entity entity = m_SelectedScene.CreateEntity("Some Entity");
    entity.AddComponent<QuadRendererComponent>(glm::vec4{1, 0, 0, 1});
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

    ImGui::Begin("Game Viewport");
    s_IsViewportSelected = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

    ImGui::BeginChild("Render"); {

        ImVec2 wsize = ImGui::GetWindowSize();
        s_Camera.SetViewportSize(wsize.x, wsize.y);

        ImGui::Image((void*)(intptr_t) *s_RenderLayer.GetRenderTexture(), wsize, { 0.0, 1.0 }, { 1.0, 0.0 });

        // ImGuizmo
        if(s_HierarchyEditorPanel.GetSelectedEntity() && s_HierarchyEditorPanel.GetSelectedEntity().ContainsComponent<TransformComponent>()) {

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float windowWidth   = (float) ImGui::GetWindowWidth();
            float windowHeight  = (float) ImGui::GetWindowHeight();

            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            const glm::mat4& cameraProjection = s_Camera.GetProjctionMatrix();
            glm::mat4 cameraView = s_Camera.GetViewMatrix();

            auto& transformComponent = s_HierarchyEditorPanel.GetSelectedEntity().GetComponent<TransformComponent>();

            glm::mat4 transform = transformComponent.Transform.GetTransformationMatrix();

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(transform));

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

    // ImGui Layer
    s_ImGuiLayer.Begin();
    ImGuizmo::BeginFrame();

    DrawDockspace(); // Draw the dockspace environment
    DrawGameViewport(); // Draw the game viewport
    s_HierarchyEditorPanel.OnImGuiRender();

    s_ImGuiLayer.End();

    // Render Layer
    s_RenderLayer.Render();
}   

void EditorScene::OnDestroy() {
    s_ImGuiLayer.OnDestroy();
    s_RenderLayer.OnDestroy();
}