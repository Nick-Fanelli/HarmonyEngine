#pragma once

#include <harmonypch.h>

#include <filesystem>

#include "Project.h"

using namespace HarmonyEngine;

class Application {

	friend class ProjectManager;
	Project m_Project;

public:

    static const std::string& GetApplicationSupportDirectory() {
#ifdef HARMONY_PLATFORM_MACOS
        static std::string path = Platform::GetHomeDirectory() + "/Library/Application Support/HarmonyEngine/";
#else
        static std::string path = Platform::GetHomeDirectory() + ".harmonyengine/";
#endif

		if(!FileUtils::FileExists(path))
			std::filesystem::create_directories(path);

        return path;
    }

	static const std::filesystem::path& GetApplicationCacheFilepath() {
#ifdef HARMONY_PLATFORM_MACOS
		static std::filesystem::path path = Platform::GetHomeDirectory() + "/Library/Caches/HarmonyEngine/harmony-cache.yaml";
#else
		static std::filesystem::path path = Platform::GetHomeDirectory() + ".harmonyengine/harmony-cache.yaml";
#endif

		if(!std::filesystem::exists(path.parent_path()))
			std::filesystem::create_directories(path.parent_path());

		return path;
	}

	const Project& GetCurrentProject() const { return m_Project; }

	static void OpenFolderDialog(const char* rootDirectory, const std::function<void(const char*)>& function);
	static void OpenFileDialog(const std::pair<const char*, const char*>& filterItems, const std::function<void(const char*)>& function);
};