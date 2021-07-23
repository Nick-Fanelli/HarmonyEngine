#include "Application.h"

#include <nfd.h>

#include <Scene/SceneManager.h>
#include <Core/Display.h>

#include "EditorScene.h"

using namespace HarmonyEngine;
using namespace HarmonyEditor;

int main() {

#if HARMONY_DEBUG
    Log::Info("Running Harmony Engine in DEBUG mode!");
#else
    Log::Info("Running Harmony Engine in RELEASE mode!");
#endif

    Log::SetLogLevel(Log::LogLevel::LogLevelWarn);

    // Startup
    HARMONY_PROFILE_BEGIN_SESSION("Startup", "HarmonyProfile-Startup.json");
    EditorScene editorScene{};
    Display::CreateDisplay("Harmony Engine");
    SceneManager::SetActiveScene(&editorScene);

    HARMONY_PROFILE_END_SESSION();

    // Runtime
    HARMONY_PROFILE_BEGIN_SESSION("Runtime", "HarmonyProfile-Runtime.json");
    Display::StartGameLoop();
    HARMONY_PROFILE_END_SESSION();

    // Shutdown
    HARMONY_PROFILE_BEGIN_SESSION("Shutdown", "HarmonyProfile-Shutdown.json");
    Display::CleanUp();
    HARMONY_PROFILE_END_SESSION();
}

const std::filesystem::path& Application::GetApplicationSupportDirectory() {
#ifdef HARMONY_PLATFORM_MACOS
    static std::filesystem::path path = Platform::GetHomeDirectory() + "/Library/Application Support/HarmonyEngine/";
#else
    static std::string path = Platform::GetHomeDirectory() + ".harmonyengine/";
#endif

    if(!FileUtils::FileExists(path))
        std::filesystem::create_directories(path);

    return path;
}

const std::filesystem::path& Application::GetApplicationCacheFilepath() {
#ifdef HARMONY_PLATFORM_MACOS
    static std::filesystem::path path = Platform::GetHomeDirectory() + "/Library/Caches/HarmonyEngine/harmony-cache.yaml";
#else
    static std::filesystem::path path = Platform::GetHomeDirectory() + ".harmonyengine/harmony-cache.yaml";
#endif

    if(!std::filesystem::exists(path.parent_path()))
        std::filesystem::create_directories(path.parent_path());

    return path;
}

void Application::OpenFileDialog(const std::pair<const char*, const char*>& filterItems, const std::function<void(const std::filesystem::path&)>& function) {
    NFD_Init();

    nfdchar_t* outPath;
    nfdfilteritem_t nfdFilterItems[1] = { { filterItems.first, filterItems.second } };
    nfdresult_t result = NFD_OpenDialog(&outPath, nfdFilterItems, 1, nullptr);

    if(result == NFD_OKAY) {
        auto path = std::filesystem::path(outPath);

        function(path);
        NFD_FreePath(outPath);
    }

    NFD_Quit();
}
