#pragma once

#include <harmonypch.h>

#include "Project.h"

class Application {

	friend class ProjectManager;
	Project m_Project;

public:

    static const std::string& GetApplicationSupportDirectory() {
#ifdef HARMONY_PLATFORM_MACOS
        static std::string path = Platform::GetHomeDirectory() + "/Library/Application Support/HarmonyEngine/";
#else
        static std::string = Platform::GetHomeDirectory() + ".harmonyengine/";
#endif
        return path;
    }

	const Project& GetCurrentProject() const { return m_Project; }

	static void OpenFolderDialog(const char* rootDirectory, const std::function<void(const char*)>& function);
	static void OpenFileDialog(const std::pair<const char*, const char*>& filterItems, const std::function<void(const char*)>& function);
};