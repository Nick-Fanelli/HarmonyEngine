#include "ImGuiLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/misc/cpp/imgui_stdlib.h>

#include <ImGuizmo.h>

#include <Core/Display.h>
#include <Render/Renderer2D.h>

#include "MenuBarLayer.h"

#include "../Application.h"
#include "../Project.h"

#include "../Theme.h"

#include "../EditorPanels/HierarchyEditorPanel.h"
#include "../EditorPanels/InspectorEditorPanel.h"
#include "../EditorPanels/AssetsEditorPanel.h"

#define ICON_MIN_FA 0xe005
#define ICON_MAX_FA 0xf8ff

static constexpr float ColumnWidth = 75.0f;

static EditorScene* s_EditorScenePtr;
static Settings* s_SettingsPtr;
static std::string s_SaveFilename;

static HierarchyEditorPanel s_HierarchyEditorPanel;
static InspectorEditorPanel s_InspectorEditorPanel;
static AssetsEditorPanel       s_AssetsEditorPanel;

bool ImGuiLayer::GetIsEditorSelected() { return s_IsEditorSelected; }
Entity& ImGuiLayer::GetSelectedEntity() { return s_SelectedEntity; }

void ImGuiLayer::SetSelectedEntity(Entity& entity) { s_SelectedEntity = entity; }

static void SetDarkThemeColors() {
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

void ImGuiLayer::OnCreate(EditorScene* editorScenePtr, Settings* settings) {

    HARMONY_PROFILE_FUNCTION();

    s_EditorScenePtr = editorScenePtr;
    s_SettingsPtr = settings;

    s_SaveFilename = Application::GetApplicationSupportDirectory() + "/window-layout.ini";

    const char* glslVersion = "#version 410 core";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = s_SaveFilename.c_str();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImFontConfig config = ImFontConfig();
    config.OversampleH = 3;
    config.OversampleV = 3;

    io.FontDefault = io.Fonts->AddFontFromFileTTF("engineAssets/fonts/segoe-ui.ttf", 18.0f, &config);

    // Font Awesome
    ImFontConfig faConfig;
    faConfig.MergeMode = true;
    faConfig.GlyphMinAdvanceX = 13.0f;
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromFileTTF( "engineAssets/fonts/font-awesome/Font-Awesome-Solid-900.otf", 16.0f, &faConfig, icons_ranges );

    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    SetDarkThemeColors();

    Display::GetImGuiSize(&io.DisplaySize); // Set the ImGui display size

    ImGui_ImplGlfw_InitForOpenGL(Display::GetWindowPtr(), true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    MenuBarLayer::OnCreate();

    s_HierarchyEditorPanel.OnCreate(s_EditorScenePtr);
    s_InspectorEditorPanel.OnCreate(s_EditorScenePtr);
    s_AssetsEditorPanel.OnCreate(s_EditorScenePtr);
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
    ImGuizmo::BeginFrame();
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
        s_EditorScenePtr->GetEditorCamera().SetViewportSize(wsize.x, wsize.y);

        ImGui::Image((void*)(intptr_t) *s_EditorScenePtr->GetRenderTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

        if(s_SelectedEntity.IsCreated() && s_SelectedEntity.ContainsComponent<TransformComponent>()) {

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float windowWidth = (float) ImGui::GetWindowWidth();
            float windowHeight = (float) ImGui::GetWindowHeight();

            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            auto& camera = s_EditorScenePtr->GetEditorCamera();
            const glm::mat4& cameraProjection = camera.GetProjctionMatrix();
            glm::mat4 cameraView = camera.GetViewMatrix();

            auto& transformComponent = s_SelectedEntity.GetComponent<TransformComponent>();

            glm::mat4 transform = transformComponent.Transform.GetTransformationMatrix();

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
                ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(transform));

            if(ImGuizmo::IsUsing()) {

                glm::vec3 rotation;
                MathUtils::DecomposeMatrix(transform, transformComponent.Transform.Position, rotation, transformComponent.Transform.Scale);

                glm::vec3 deltaRotation = rotation - transformComponent.Transform.Rotation;
                transformComponent.Transform.Rotation += deltaRotation;
            }
        }

        ImGui::EndChild();
    }

    ImGui::End();
}

static void DrawAssetsStats() {
    ImGui::Begin("Assets Stats", Settings::GetShowAssetsStatsPtr());

    if(ImGui::TreeNode("Currently Loaded Assets")) {

        static constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

        if(ImGui::TreeNode("Textures")) {
            for(auto& texture : AssetManager::GetTextureRegistry())
                ImGui::TreeNodeEx(texture.GetFilepath().c_str(), flags);

            ImGui::TreePop();
        }

        if(ImGui::TreeNode("Meshes")) {

            for(auto& mesh : AssetManager::GetMeshRegistry())
                ImGui::TreeNodeEx(mesh.Filepath.c_str(), flags);

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

void ImGuiLayer::OnUpdate() {

    HARMONY_PROFILE_FUNCTION();

    StartFrame();

    ImGuiStyle& style = ImGui::GetStyle();

    float minWindowSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    DrawDockspace();
    style.WindowMinSize.x = minWindowSizeX;

    MenuBarLayer::OnUpdate();

    s_HierarchyEditorPanel.OnUpdate();
    s_InspectorEditorPanel.OnUpdate();
    s_AssetsEditorPanel.OnUpdate();

    ShowGameViewport();

    if(Settings::ShouldShowRendererStats())
        RendererStats::DrawImGUIStats(Settings::GetShowRendererStatsPtr());

    if(Settings::ShouldShowEnvironmentSettings())
        Renderer::DrawImGuiEnvironmentLighting(Settings::GetShowEnvironmentSettingsPtr());

    if(Settings::ShouldShowAssetsStats())
        DrawAssetsStats();

    s_SettingsPtr->OnImGuiRender();

    ProjectManager::OnImGuiRender();

    EndFrame();
}

void ImGuiLayer::OnDestroy() {
    ImGui::SaveIniSettingsToDisk(s_SaveFilename.c_str());
}

void ImGuiLayer::DrawBool(const std::string& label, bool& value) {
    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::Checkbox("", &value);

    ImGui::PopID();
}


void ImGuiLayer::DrawInteger(const std::string& label, int& value, float speed, int min, int max) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::DragInt("", &value, speed, min, max);
    ImGui::Columns(1);
   
    ImGui::PopID();
}

void ImGuiLayer::DrawFloat(const std::string& label, float& value, float speed, float min, float max, const std::string& format) {
    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::DragFloat("", &value, speed, min, max, format.c_str());

    ImGui::PopID();
}

void ImGuiLayer::DrawFloat(const std::string& label, float& value, float speed, float min, float max) {
    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::DragFloat("", &value, speed, min, max);

    ImGui::PopID();
}

void ImGuiLayer::DrawInput(const std::string& label, std::string& value) {
    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::InputText("", &value);

    ImGui::PopID();
}

void ImGuiLayer::DrawVector3(const std::string& label, glm::vec3& values, float resetValue, float columnWidth) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}

void ImGuiLayer::DrawColorControl(const std::string& label, glm::vec4& values, float resetValue) {
    // ImGui::ColorEdit4(label.c_str(), glm::value_ptr(values));
    ImGui::PushID(label.c_str());

    bool changed = false;

    // glm::vec4 tempColor = { values.r * 255.0f, values.g * 255.0f, values.b * 255.0f, values.a * 255.0f };
    glm::vec4 tempColor = glm::vec4(values) * 255.0f;

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    
    ImGui::PushMultiItemsWidths(5, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    // ImGui::PushFont(boldFont);
    if(ImGui::Button("R", buttonSize))
        tempColor.r = resetValue;
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragFloat("##R", &tempColor.r, 1.0f, 0.0f, 255.0f, "%.0f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("G", buttonSize))
        tempColor.g = 0.0f;
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragFloat("##G", &tempColor.g, 1.0f, 0.0f, 255.0f, "%.0f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("B", buttonSize))
        tempColor.b = resetValue;
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragFloat("##B", &tempColor.b, 1.0f, 0.0f, 255.0f, "%.0f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });
    // ImGui::PushFont(boldFont);
    if (ImGui::Button("A", buttonSize))
        tempColor.a = resetValue;
    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragFloat("##A", &tempColor.a, 1.0f, 0.0f, 255.0f, "%.0f");
    ImGui::PopItemWidth();
    
    ImGui::PopStyleVar();

    if(changed)
        values = tempColor / 255.0f;

    ImGui::SameLine();
    if(ImGui::ColorButton("##ColorButton", ImVec4{ values.r, values.g, values.b, values.a })) {
        ImGui::OpenPopup("picker");
    }

    if (ImGui::BeginPopup("picker")) {
        ImGui::ColorPicker4("##picker", glm::value_ptr(values));
        ImGui::EndPopup();
    }
    
    ImGui::Columns(1);

    ImGui::PopID();
}

void ImGuiLayer::DrawTextureInputControl(const std::string& label, AssetHandle<Texture>& assetHandle) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::Button(assetHandle.IsAssigned() ? assetHandle.GetAssetBinding().AssetName.c_str() : "[Unattached]");
    if(ImGui::BeginDragDropTarget()) {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(AssetsEditorPanel::TextureDragDropID)) {
            AssetHandle<Texture> texture = *(const AssetHandle<Texture>*) payload->Data;
            assetHandle = texture;
            if(!assetHandle->IsCreated()) {
                assetHandle->Create();
            }
        }

        ImGui::EndDragDropTarget();
    }

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::SameLine();

    if(ImGui::Button("\uf0e2", { lineHeight, lineHeight })) {
        if(assetHandle.IsAssigned()) {
            assetHandle = AssetHandle<Texture>(nullptr);
        }
    }

    ImGui::Columns(1);

    ImGui::PopID();
}

void ImGuiLayer::DrawMeshInputControl(const std::string& label, AssetHandle<Mesh>& assetHandle) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::Button(assetHandle.IsAssigned() ? assetHandle.GetAssetBinding().AssetName.c_str() : "[Unattached]");
    if(ImGui::BeginDragDropTarget()) {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(AssetsEditorPanel::MeshDragDropID)) {
            AssetHandle<Mesh> mesh = *(const AssetHandle<Mesh>*) payload->Data;
            assetHandle = mesh;
            if(!assetHandle->IsCreated()) {
                Renderer::LoadOBJFile(assetHandle->Filepath, assetHandle.GetAsset());
            }
        }

        ImGui::EndDragDropTarget();
    }

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::SameLine();

    if(ImGui::Button("\uf0e2", { lineHeight, lineHeight })) {
        if(assetHandle.IsAssigned()) {
            assetHandle = AssetHandle<Mesh>(nullptr);
        }    
    }

    ImGui::Columns(1);

    ImGui::PopID();
}