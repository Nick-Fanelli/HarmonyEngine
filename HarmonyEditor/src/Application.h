#pragma once

#include <harmonypch.h>

#include <filesystem>

using namespace HarmonyEngine;

class Application {

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
};