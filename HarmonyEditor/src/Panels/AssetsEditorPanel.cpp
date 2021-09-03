#include "AssetsEditorPanel.h"

#include <imguipch.h>

#include <time.h>

#include "../EditorLayer.h"
#include "../Settings.h"

#include "../ImGuiDefaults.h"
#include "../Windows/CommonWindows.h"

using namespace HarmonyEditor;

static AssetsEditorPanel::AssetFile s_RootFile;
static time_t s_Timer = time(0);

static EditorLayer* s_EditorLayerPtr = nullptr;

AssetsEditorPanel::AssetsEditorPanel(EditorLayer* editorLayerPtr) : m_EditorLayerPtr(editorLayerPtr) {
    s_EditorLayerPtr = editorLayerPtr;

    SyncAssets();
}

static void LoadFile(AssetsEditorPanel::AssetFile& parent) {
    HARMONY_PROFILE_FUNCTION();

    for(const auto& childEntry : std::filesystem::directory_iterator(parent.Filepath)) {
        if(childEntry.is_directory()) {
            auto& childAssetFile = parent.Children.emplace_back(childEntry.path(), AssetsEditorPanel::AssetTypeDirectory);
            LoadFile(childAssetFile);
        } else {

            static const std::regex imageRegex("[^\\s]+(.*?)\\.(jpg|jpeg|png|gif|bmp|tga|psd|hdr|pic|pnm|JPG|JPEG|PNG|GIF|BMP|TGA|PSD|HDR|PIC|PNM)$");

            auto& path = childEntry.path();
            auto extension = path.extension();

            if(extension == ".hyscene") {
                parent.Children.emplace_back(path, AssetsEditorPanel::AssetTypeHarmonyScene);
            } else if(extension == ".lua") {
                parent.Children.emplace_back(path, AssetsEditorPanel::AssetTypeLuaScript);
            } else if(std::regex_match(path.c_str(), imageRegex)) {
                parent.Children.emplace_back(path, AssetsEditorPanel::AssetTypeTexture);
            } else {
                parent.Children.emplace_back(path, AssetsEditorPanel::AssetTypeUnknown);
            }
        }
    }
}

static std::filesystem::path s_TempPath;

void AssetsEditorPanel::DrawFileImGui(const std::filesystem::path& parentPath, AssetFile& child) {
    HARMONY_PROFILE_FUNCTION();

    static constexpr auto flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    bool isOpened = false; 

    switch(child.AssetType) {
        case AssetTypeDirectory:
            isOpened = ImGui::TreeNode(child.Filepath.c_str(), "\uf07b %s", child.GetRelativePath(parentPath).c_str());
            break;
        case AssetTypeHarmonyScene:
            ImGui::TreeNodeEx(child.Filepath.c_str(), flags, "\uf466 %s", child.GetRelativePath(parentPath).c_str());

            if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
                s_EditorLayerPtr->OpenScene(child.Filepath);
            }

            if(ImGui::BeginDragDropSource()) {
                s_TempPath = child.Filepath;

                ImGui::SetDragDropPayload(ImGuiDefaults::HarmonyScenePathDragDropID, &s_TempPath, sizeof(s_TempPath), ImGuiCond_Once);

                ImGui::Text("%s %s", "\uf466", child.Filepath.filename().c_str());
                
                ImGui::EndDragDropSource();
            }

            break;
        case AssetTypeTexture:
            ImGui::TreeNodeEx(child.Filepath.c_str(), flags, "\uf1c5 %s", child.GetRelativePath(parentPath).c_str());

            if(ImGui::BeginDragDropSource()) {
                s_TempPath = child.Filepath;

                ImGui::SetDragDropPayload(ImGuiDefaults::TexturePathDragDropID, &s_TempPath, sizeof(s_TempPath), ImGuiCond_Once);

                ImGui::Text("%s %s", "\uf1c5", child.Filepath.filename().c_str());

                ImGui::EndDragDropSource();
            }

            break;

        case AssetTypeLuaScript:

            ImGui::TreeNodeEx(child.Filepath.c_str(), flags, "\uf1c9 %s", child.GetRelativePath(parentPath).c_str());

            if(ImGui::BeginDragDropSource()) {
                s_TempPath = child.Filepath;

                ImGui::SetDragDropPayload(ImGuiDefaults::LuaScriptPathDragDropID, &s_TempPath, sizeof(s_TempPath), ImGuiCond_Once);

                ImGui::Text("%s %s", "\uf1c9", child.Filepath.filename().c_str());

                ImGui::EndDragDropSource();
            }

            break;
        case AssetTypeUnknown:
            ImGui::TreeNodeEx(child.Filepath.c_str(), flags, "\uf15b %s", child.GetRelativePath(parentPath).c_str());
            break;
    }

    if(ImGui::BeginPopupContextItem(child.Filepath.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if(ImGui::Selectable("Delete")) {
            ConfirmationWindow::Confirm("Are you sure you want to delete this file? It can not be returned!", [this, child]() {
                if(!std::filesystem::remove_all(child.Filepath)) {
                    Log::Warn("Didn't Delete File");
                }

                SyncAssets();
            });
        }

        ImGui::EndPopup();
    }

    if(isOpened) {
        for(auto& file : child.Children)
            DrawFileImGui(child.Filepath, file);

        ImGui::TreePop();
    }
    
}

void AssetsEditorPanel::SyncAssets() {
    HARMONY_PROFILE_FUNCTION();
    
    if(m_EditorLayerPtr->GetActiveProject().IsAssigned()) {

        s_RootFile.Children.clear();

        s_RootFile = { m_EditorLayerPtr->GetActiveProject().GetProjectAssetsDirectory(), AssetTypeDirectory };
        LoadFile(s_RootFile);

    }
}

void AssetsEditorPanel::OnImGuiRender() {
    HARMONY_PROFILE_FUNCTION();

    if(Settings::ShowAssetsPanel) {

        if(m_EditorLayerPtr->GetActiveProject().IsAssigned()) {
            if(difftime(time(0), s_Timer) >= 1) { // TODO: Configure as setting
                SyncAssets();
                s_Timer = time(0);
            }
        }

        ImGui::Begin("Assets Browser", &Settings::ShowAssetsPanel.CurrentValue);

        for(auto& child : s_RootFile.Children) {
            DrawFileImGui(m_EditorLayerPtr->GetActiveProject().GetProjectAssetsDirectory(), child);
        }

        ImGui::BeginChild("RightClickSelectable", ImGui::GetContentRegionAvail());        ImGui::EndChild();
        if(ImGui::BeginPopupContextItem("AssetsEditorPanelRightClickArea", ImGuiPopupFlags_MouseButtonRight)) {
            if(ImGui::Selectable("New Folder")) {
                auto path = m_EditorLayerPtr->GetActiveProject().GetProjectAssetsDirectory() / "Untitled Folder";
                
                uint32_t i = 1;

                while(std::filesystem::exists(path)) {
                    path = m_EditorLayerPtr->GetActiveProject().GetProjectAssetsDirectory() / (std::string("Untitled Folder ") + std::to_string(i));
                    if(i > 2000) {
                        Log::Error("You are kidding me!!! How do you have 2,000 untitled folders!!!?!?!?!\n\tStatus: Breaking");
                        break; 
                    }

                    i++;
                }

                std::filesystem::create_directories(path);
                SyncAssets();
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }
}