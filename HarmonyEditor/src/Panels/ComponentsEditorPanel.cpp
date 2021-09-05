#include "ComponentsEditorPanel.h"

#include <Scene/Entity.h>

#include "../ImGuiDefaults.h"
#include "../Settings.h"

#include "../Windows/CommonWindows.h"

using namespace HarmonyEditor;

template<typename ComponentType, typename UIFunction>
static void DrawComponent(const char* componentName, Entity& entity, UIFunction uiFunction) {

    HARMONY_PROFILE_FUNCTION();

    ImGui::PushID(componentName);

    static constexpr auto flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    if(entity.ContainsComponent<ComponentType>()) {
        
        ImGui::Separator();

        const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        auto contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2.0f, 3.0f });
        bool opened = ImGui::TreeNodeEx(componentName, flags);
        ImGui::PopStyleVar();

        ImGui::SameLine(contentRegionAvailable.x - lineHeight / 2.0f);

        if(ImGui::Button("\uf068", { lineHeight, lineHeight })) {
            entity.RemoveComponent<ComponentType>();
            if(opened) ImGui::TreePop();
            ImGui::PopID();
            return;
        }

        if(opened) {
            auto& component = entity.GetComponent<ComponentType>();
            uiFunction(component);
            ImGui::TreePop();
        }
    }

    ImGui::PopID();
}

static void DisplayEntity(Entity& entity) {

    if(!entity.ContainsComponent<TagComponent>())
        return;

    auto& tagComponent = entity.GetComponent<TagComponent>();
    ImGuiDefaults::DrawTextInput("Entity Name", tagComponent.Name);

    DrawComponent<TransformComponent>("Transform", entity, [&](TransformComponent& component) {
        ImGuiDefaults::DrawVector3("Position", component.Transform.Position);
        ImGuiDefaults::DrawVector3("Rotation", component.Transform.Rotation, 0.0f);
        ImGuiDefaults::DrawVector3("Scale", component.Transform.Scale, 1.0f);
    });

    DrawComponent<OrthographicCameraComponent>("Orthographic Camera", entity, [](OrthographicCameraComponent& component) {
        ImGuiDefaults::DrawFloat("Zoom", component.Camera.GetZoomRef(), 0.1f, static_cast<float>(NULL), static_cast<float>(NULL), "%.1f");

        ImGui::Spacing();

        ImGuiDefaults::DrawFloat("Near Clip", component.Camera.GetNearClipRef(), 0.1f, 0.0f);
        ImGuiDefaults::DrawFloat("Far Clip", component.Camera.GetFarClipRef(), 0.1f, 0.0f);
    });

    DrawComponent<QuadRendererComponent>("Quad Renderer", entity, [](QuadRendererComponent& component) {  
        ImGuiDefaults::DrawColorControl("Color", component.Color);
        ImGuiDefaults::DrawTextureControl("Texture", component.TextureHandle);
    });

    DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& component) {
        ImGuiDefaults::DrawColorControl("Color", component.Color);
        ImGuiDefaults::DrawTextureControl("Texture", component.TextureHandle);

        ImGuiDefaults::PushColumnWidth(150.0f);

        ImGuiDefaults::DrawVector2("Top Left Coord", component.TopLeftCoord, 0.0f, 0.0f, 1.0f);
        ImGuiDefaults::DrawVector2("Bottom Right Coords", component.BottomRightCoord, 1.0f, 0.0f, 1.0f);

        ImGuiDefaults::PopColumnWidth();
    });

    DrawComponent<LuaScriptComponent>("Lua Script", entity, [](LuaScriptComponent& component) {
        ImGuiDefaults::DrawLuaScriptControl("Lua Script", component.Script);
    });

    DrawComponent<PointLightComponent>("Point Light", entity, [](PointLightComponent& component) {
        ImGuiDefaults::DrawColorControl("Hue", component.Hue);
        ImGuiDefaults::DrawFloat("Intensity", component.Intensity, 0.01f, 0.0f);
    });

    // New Component Button
    ImGui::Separator();
    if(ImGui::Button("Add Component"))
        NewComponentWindow::OpenNewComponentPopup(entity);
}

void ComponentsEditorPanel::DisplayScenePreferences() {

    static constexpr auto flags = ImGuiTreeNodeFlags_DefaultOpen;

    static Scene* selectedScene = SceneManager::GetActiveScenePtr();

    ImGuiDefaults::DrawTextInput("Scene Name", selectedScene->GetSceneName());
    ImGui::Separator();

    if(ImGui::TreeNodeEx("Renderer Settings", flags)) {
        ImGui::Indent();

        ImGuiDefaults::DrawFloat("Ambient Intensity", selectedScene->GetAmbientIntensity(), 0.1f, 0.0f, 1.0f);

        ImGui::Unindent();
        ImGui::TreePop();
    }

    ImGui::Separator();

    if(ImGui::TreeNodeEx("Global Scripts", flags)) {
        ImGui::Indent();

        ImGuiDefaults::PushColumnWidth(100.0f);
        ImGuiDefaults::DrawLuaScriptControl("Global Script", selectedScene->GetGlobalScript());
        ImGuiDefaults::PopColumnWidth();

        ImGui::Unindent();
        ImGui::TreePop();
    }

}

void ComponentsEditorPanel::OnImGuiRender() {
    if(Settings::ShowComponentsPanel) {

        ImGui::Begin("Components", &Settings::ShowComponentsPanel.CurrentValue);

        if(m_HierarchyEditorPanelPtr->GetSelectedEntity())
            DisplayEntity(m_HierarchyEditorPanelPtr->GetSelectedEntity());
        else if(m_HierarchyEditorPanelPtr->IsSceneSelected())
            DisplayScenePreferences();

        ImGui::End();

    }
}