#pragma once

#include <harmonypch.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

	class Application {

	public:

		static const std::string& GetDisplayTitle() {
			static const std::string title = "Harmony Engine"; // TODO: Add version or some other information
			return title;
		}

		static const std::filesystem::path& GetApplicationSupportDirectory();
		static const std::filesystem::path& GetApplicationCacheFilepath();

		static void OpenFileDialog(const std::pair<const char*, const char*>& filterItems, const std::function<void(const std::filesystem::path&)>& function);
		static void OpenFolderDialog(const std::function<void(const std::filesystem::path&)>& function);

		static void SaveFileDialog(const std::pair<const char*, const char*>& filterItems, const std::function<void(const std::filesystem::path&)>& function);

	};

}