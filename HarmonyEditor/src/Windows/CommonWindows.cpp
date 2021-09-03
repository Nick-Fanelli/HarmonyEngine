#include "CommonWindows.h"

#include <Core/Input.h>

using namespace HarmonyEditor;

void CommonWindows::OnImGuiRender() {
    ConfirmationWindow::OnImGuiRender();
    NewComponentWindow::OnImGuiRender();
}

// Confirmation Window

const char* ConfirmationWindow::s_PopupWindowTitle = "Confirmation Window";

bool ConfirmationWindow::s_ShouldOpen = false;
bool ConfirmationWindow::s_UseVoidFunctionPtr = false;

std::string ConfirmationWindow::s_PopupMessage;

std::function<void(bool)> ConfirmationWindow::s_FunctionPtr;
std::function<void()> ConfirmationWindow::s_VoidFunctionPtr;

void ConfirmationWindow::Confirm(const std::string& message, std::function<void(bool)> functionPtr) {
    s_PopupMessage = message;
    s_FunctionPtr = functionPtr;
    s_UseVoidFunctionPtr = false;

    s_ShouldOpen = true;
}

void ConfirmationWindow::Confirm(const std::string& message, std::function<void()> functionPtr) {
    s_PopupMessage = message;
    s_VoidFunctionPtr = functionPtr;
    s_UseVoidFunctionPtr = true;

    s_ShouldOpen = true;
}

void ConfirmationWindow::OnImGuiRender() {

    static constexpr auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings 
                                | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    if(s_ShouldOpen) {
        ImGui::OpenPopup(s_PopupWindowTitle);
        ImGui::SetNextWindowSize({ 300.0f, 125.0f }, ImGuiCond_Appearing);

        s_ShouldOpen = false;
    }

    if(ImGui::BeginPopupModal(s_PopupWindowTitle, NULL, flags)) {

        ImGui::TextWrapped("%s", s_PopupMessage.c_str());

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 35.0f); // 10px padding

        if(ImGui::Button("Cancel", { 75.0f, 25.0f })) {
            ImGui::CloseCurrentPopup();
            if(!s_UseVoidFunctionPtr)
                s_FunctionPtr(false);
        }

        ImGui::SameLine();

        if(ImGui::Button("Confirm", { 75.0f, 25.0f })) {
            ImGui::CloseCurrentPopup();
            if(s_UseVoidFunctionPtr)
                s_VoidFunctionPtr();
            else
                s_FunctionPtr(true);
        }

        ImGui::EndPopup();
    }
}

// New Component Windows
bool NewComponentWindow::s_IsVisable = false;
Entity NewComponentWindow::s_CurrentEntity = {};

static const char* s_SelectedComponentID = nullptr;
static bool s_ShouldApplyComponent = false;

void NewComponentWindow::OpenNewComponentPopup(Entity& entity) {
    if(!entity)
        return;
    
    s_SelectedComponentID = nullptr;
    s_ShouldApplyComponent = false;
    s_CurrentEntity = entity;
    s_IsVisable = true;
}

void NewComponentWindow::CloseNewComponentPopup() {
    s_IsVisable = false;
}

template<typename ComponentType>
static void DrawComponent(const char* componentName, Entity& entity, ImGuiTextFilter& filter) {

    static constexpr auto treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    if(!entity.ContainsComponent<ComponentType>() && filter.PassFilter(componentName)) {
        ImGui::TreeNodeEx(componentName, treeNodeFlags | ((s_SelectedComponentID == componentName) ? ImGuiTreeNodeFlags_Selected : 0));

        if(s_ShouldApplyComponent && s_SelectedComponentID == componentName) {
            entity.AddComponent<ComponentType>();
            NewComponentWindow::CloseNewComponentPopup();
        }

        if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            entity.AddComponent<ComponentType>();
            NewComponentWindow::CloseNewComponentPopup();
        }

        if(ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
            s_SelectedComponentID = componentName;
        }
    }

}

void NewComponentWindow::OnImGuiRender() {    

    // ImGui::ShowDemoWindow();

    if(s_IsVisable) {

        static constexpr auto flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowSize({800.0f, 550.0f}, ImGuiCond_Appearing);
        ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->GetCenter().x - 400.0f, ImGui::GetMainViewport()->GetCenter().y - 275.0f }, ImGuiCond_Appearing);

        ImGui::Begin("New Component", &s_IsVisable, flags);

        ImGui::BeginChild("##Component List", { ImGui::GetWindowWidth() / 2.0f, ImGui::GetContentRegionAvail().y }, false, flags); {

            static ImGuiTextFilter filter;
    
            // Draw Search Bar
            ImGui::Text("Search");
            ImGui::SameLine();
            filter.Draw("", ImGui::GetContentRegionAvailWidth());

            DrawComponent<TransformComponent>("Transform", s_CurrentEntity, filter);
            DrawComponent<CameraComponent>("Camera", s_CurrentEntity, filter);
            DrawComponent<QuadRendererComponent>("Quad Renderer", s_CurrentEntity, filter);
            DrawComponent<SpriteRendererComponent>("Sprite Renderer", s_CurrentEntity, filter);
            DrawComponent<LuaScriptComponent>("Lua Script", s_CurrentEntity, filter);
            DrawComponent<PointLightComponent>("Point Light", s_CurrentEntity, filter);

        } ImGui::EndChild();

        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::SameLine();

        ImGui::BeginChild("Description View", ImGui::GetContentRegionAvail(), false, flags | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse); {

            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

            ImGui::BeginChild("##Description", { ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y - lineHeight - 5.0f }, false, flags);

            if(!s_SelectedComponentID)
                // TODO: Implement somthing where it shows documentation for how components work
                ImGui::Text("%s", "Select A Component for Documentation");
            else   
                // Pull the specific component documentation from some wiki
                ImGui::Text("%s", "Component Documentation Comming Soon!");

            ImGui::EndChild();

            static const auto adjustment = ImGui::CalcTextSize("Add Component").x + 15.0f;

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - adjustment);

            if(s_SelectedComponentID == nullptr)
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);

            s_ShouldApplyComponent |= ImGui::ButtonEx("Add Component", {}, (s_SelectedComponentID == nullptr ? ImGuiItemFlags_Disabled : 0));

            if(Input::IsKeyDown(HARMONY_KEY_ENTER))
                s_ShouldApplyComponent = true;

            if(s_SelectedComponentID == nullptr) {
                s_ShouldApplyComponent = false;
                ImGui::PopStyleVar();
            }

        } ImGui::EndChild();

        ImGui::End();
    }
}