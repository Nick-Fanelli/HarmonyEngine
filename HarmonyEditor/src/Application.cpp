#include "Application.h"

#include <Core/Display.h>
#include <Scene/SceneManager.h>

#include "Scenes/EditorScene.h"

#include <nfd.h>

int main() {

    Log::SetLogLevel(Log::LogLevelWarn);

#if HARMONY_DEBUG
    std::cout << "Running Harmony Engine in DEBUG mode!" << std::endl;
#else
    std::cout << "Running Harmony Engine in RELEASE mode!" << std::endl;
#endif

    // Startup
    HARMONY_PROFILE_BEGIN_SESSION("Startup", "HarmonyProfile-Startup.json");

    EditorScene launchScene = EditorScene();
    Display::CreateDisplay("Harmony Engine");
    SceneManager::SetActiveScene(&launchScene);

    HARMONY_PROFILE_END_SESSION();
    
    // Runtime
    HARMONY_PROFILE_BEGIN_SESSION("Runtime", "HarmonyProfile-Runtime.json");
    Display::StartGameLoop();
    HARMONY_PROFILE_END_SESSION();

    ProjectManager::SaveCurrentProject();

    // Shutdown
    HARMONY_PROFILE_BEGIN_SESSION("Shutdown", "HarmonyProfile-Shutdown.json");
    Display::CleanUp();
    HARMONY_PROFILE_END_SESSION();

}

void Application::OpenFolderDialog(const char* rootDirectory, const std::function<void(const char*)>& function) {
    NFD_Init();

    nfdchar_t* outPath;
    nfdresult_t result = NFD_PickFolder(&outPath, rootDirectory);
    if(result == NFD_OKAY) {
        function(outPath);
        NFD_FreePath(outPath);
    }

    NFD_Quit();
}

void Application::OpenFileDialog(const std::pair<const char*, const char*>& filterItems, const std::function<void(const char*)>& function) {
    NFD_Init();

    nfdchar_t* outPath;
    nfdfilteritem_t filterItem[1] = { { filterItems.first, filterItems.second } };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, nullptr);
    if(result == NFD_OKAY) {
        function(outPath);
        NFD_FreePath(outPath);
    } 

    NFD_Quit();
}
