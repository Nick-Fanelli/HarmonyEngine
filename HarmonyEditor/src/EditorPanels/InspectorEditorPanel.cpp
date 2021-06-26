#include "InspectorEditorPanel.h"

#include <glm/gtc/type_ptr.hpp>

#include "../Layers/ImGuiLayer.h"
#include "AssetsEditorPanel.h"

static void DrawTextureInputControl(const std::string& label, AssetHandle<Texture>& assetHandle) {
    ImGui::Button(assetHandle.IsAssigned() ? assetHandle->GetFilepath().c_str() : "[Unattached]");
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

    ImGui::PushFont(ImGuiLayer::GetFontAwesome());

    if(ImGui::Button("\uf0e2##DrawTextureInputControl", { lineHeight, lineHeight })) {
        if(assetHandle.IsAssigned()) {
            assetHandle = AssetHandle<Texture>(nullptr);
        }
    }

    ImGui::PopFont();

    ImGui::SameLine();
    ImGui::Text("%s", label.c_str());
}

static void DrawMeshInputControl(const std::string& label, AssetHandle<Mesh>& assetHandle) {
    ImGui::Button(assetHandle.IsAssigned() ? assetHandle->Filepath.c_str() : "[Unattached]");
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

    ImGui::PushFont(ImGuiLayer::GetFontAwesome());

    if(ImGui::Button("\uf0e2##DrawMeshInputControl", { lineHeight, lineHeight })) {
        if(assetHandle.IsAssigned()) {
            assetHandle = AssetHandle<Mesh>(nullptr);
        }    
    }

    ImGui::PopFont();

    ImGui::SameLine();
    ImGui::Text("%s", label.c_str());
}

template<typename ComponentType, typename UIFunction>
static void DrawComponent(const char* label, Entity& selectedEntity, UIFunction uiFunction) {

    HARMONY_PROFILE_FUNCTION();
            
    static constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    if(selectedEntity.ContainsComponent<ComponentType>()) {
        auto& component = selectedEntity.GetComponent<ComponentType>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 2, 2 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        ImGui::Separator();

        bool open = ImGui::TreeNodeEx(label, treeNodeFlags);

        ImGui::PopStyleVar();

        if(!std::is_same<ComponentType, TagComponent>()) {

            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

            ImGui::PushFont(ImGuiLayer::GetFontAwesome());
            if(ImGui::Button("\uf068", ImVec2{ lineHeight, lineHeight })) {
                selectedEntity.RemoveComponenet<ComponentType>();
            }
            ImGui::PopFont();
        }

        if(open) {
            uiFunction(component);
            ImGui::TreePop();
        }
    }
}

template<typename ComponentType>
static void DrawAddComponentMenuItem(const std::string& componentType, Entity& entity) {
    if(!entity.ContainsComponent<ComponentType>()) {
        if(ImGui::MenuItem(componentType.c_str())) {
            entity.AddComponent<ComponentType>();
            ImGui::CloseCurrentPopup();
        }
    }
}

static void DrawAddComponentButton(Entity& entity) {
    ImGui::Separator();
    if(ImGui::Button("Add Component")) 
        ImGui::OpenPopup("AddComponent");

    if(ImGui::BeginPopup("AddComponent")) {
        DrawAddComponentMenuItem<TransformComponent>("Transform", entity);
        DrawAddComponentMenuItem<QuadRendererComponent>("Quad Renderer", entity);
        DrawAddComponentMenuItem<MeshRendererComponent>("Mesh Renderer", entity);

        ImGui::EndPopup();
    }

}

void InspectorEditorPanel::OnUpdate() {

    HARMONY_PROFILE_FUNCTION();

    ImGui::Begin("Components");

    auto& selectedEntity = ImGuiLayer::GetSelectedEntity();

    if(selectedEntity.IsCreated()) {

        DrawComponent<TagComponent>("Entity Tag", selectedEntity, [](TagComponent& component) {
            ImGuiLayer::DrawInput("Name", component.Name);
        });

        DrawComponent<TransformComponent>("Transform", selectedEntity, [](TransformComponent& component) {
            ImGuiLayer::DrawVector3("Position", component.Transform.Position);
            ImGuiLayer::DrawVector3("Rotation", component.Transform.Rotation);
            ImGuiLayer::DrawVector3("Scale", component.Transform.Scale, 1.0f);
        });

        DrawComponent<QuadRendererComponent>("Quad Renderer", selectedEntity, [](QuadRendererComponent& component) {
            ImGuiLayer::DrawColorControl("Color", component.Color);
            DrawTextureInputControl("Texture", component.TextureHandle);
        });

        DrawComponent<MeshRendererComponent>("Mesh Renderer", selectedEntity, [](MeshRendererComponent& component) {
            ImGuiLayer::DrawColorControl("Tint", component.Color);
            DrawMeshInputControl("Mesh", component.MeshHandle);
            DrawTextureInputControl("Texture", component.TextureHandle);
        });

        DrawAddComponentButton(selectedEntity);

    }

    ImGui::End();

}