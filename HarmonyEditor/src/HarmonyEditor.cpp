#include "HarmonyEditor.h"

#include <nfd.h>

#include <Application.h>

#include <Scene/SceneManager.h>
#include <Core/Display.h>

#include "EditorScene.h"

using namespace HarmonyEditor;

int main() {

#if HARMONY_DEBUG
    Log::Info("Running Harmony Engine in DEBUG mode!");
    Log::SetLogLevel(Log::LogLevel::LogLevelWarn);
#else
    Log::Info("Running Harmony Engine in RELEASE mode!");
    Log::SetLogLevel(Log::LogLevel::LogLevelError);
#endif

    Application application = Application("Harmony Engine");
    EditorScene scene = EditorScene();

    NFD_Init();

    application.StartApplication(scene);

    return 0;
}

const std::filesystem::path& HarmonyEditorApplication::GetApplicationSupportDirectory() {
#ifdef HARMONY_PLATFORM_MACOS
    static std::filesystem::path path = Platform::GetHomeDirectory() + "/Library/Application Support/HarmonyEngine/";
#else
    static std::string path = Platform::GetHomeDirectory() + ".harmonyengine/";
#endif

    if(!FileUtils::FileExists(path))
        std::filesystem::create_directories(path);

    return path;
}

const std::filesystem::path& HarmonyEditorApplication::GetApplicationCacheFilepath() {
#ifdef HARMONY_PLATFORM_MACOS
    static std::filesystem::path path = Platform::GetHomeDirectory() + "/Library/Caches/HarmonyEngine/harmony-cache.yaml";
#else
    static std::filesystem::path path = Platform::GetHomeDirectory() + ".harmonyengine/harmony-cache.yaml";
#endif

    if(!std::filesystem::exists(path.parent_path()))
        std::filesystem::create_directories(path.parent_path());

    return path;
}

void HarmonyEditorApplication::OpenFileDialog(const std::pair<const char*, const char*>& filterItems, const std::function<void(const std::filesystem::path&)>& function) {
    nfdchar_t* outPath;
    nfdfilteritem_t nfdFilterItems[1] = { { filterItems.first, filterItems.second } };
    nfdresult_t result = NFD_OpenDialog(&outPath, nfdFilterItems, 1, nullptr);

    if(result == NFD_OKAY) {
        auto path = std::filesystem::path(outPath);

        function(path);
        NFD_FreePath(outPath);
    }
}

void HarmonyEditorApplication::OpenFolderDialog(const std::function<void(const std::filesystem::path&)>& function) {
    nfdchar_t* outPath;
    nfdresult_t result = NFD_PickFolder(&outPath, nullptr);
    if(result == NFD_OKAY) {
        auto path = std::filesystem::path(outPath);

        function(path);
        NFD_FreePath(outPath);
    }
}

void HarmonyEditorApplication::SaveFileDialog(const std::pair<const char*, const char*>& filterItems, const std::function<void(const std::filesystem::path&)>& function) {
    nfdchar_t* outPath;
    nfdfilteritem_t nfdFilterItems[1] = { { filterItems.first, filterItems.second } };
    nfdresult_t result = NFD_SaveDialog(&outPath, nfdFilterItems, 1, nullptr, nullptr);

    if(result == NFD_OKAY) {
        auto path = std::filesystem::path(outPath);

        function(path);
        NFD_FreePath(outPath);
    }
}
