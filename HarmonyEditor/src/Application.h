#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

	class Application {

	public:

		static const std::filesystem::path& GetApplicationSupportDirectory();
		static const std::filesystem::path& GetApplicationCacheFilepath();

		static void OpenFileDialog(const std::pair<const char*, const char*>& filterItems, const std::function<void(const std::filesystem::path&)>& function);
		static void OpenFolderDialog(const std::function<void(const std::filesystem::path&)>& function);

		static void SaveFileDialog(const std::pair<const char*, const char*>& filterItems, const std::function<void(const std::filesystem::path&)>& function);

	};

}