#pragma once

#include <harmonypch.h>

class Application {

public:

    static const std::string& GetApplicationSupportDirectory() {
#ifdef HARMONY_PLATFORM_MACOS
        static std::string path = Platform::GetHomeDirectory() + "/Library/Application Support/HarmonyEngine/";
#else
        static std::string = Platform::GetHomeDirectory() + ".harmonyengine/";
#endif
        return path;
    }

};