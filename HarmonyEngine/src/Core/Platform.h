#pragma once

#include <string>

#if defined(_WIN64) || defined(_WIN32) // Windows
    #define HARMONY_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__) // Apple
    // Check what Apple OS we are on
    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR == 1 // IPhone Simulator
        #error "IOS Simulator is not currently supported!"
    #elif TARGET_OS_IPHONE == 1 // IPhone
        #define HARMONY_PLATFORM_IOS
        #error "IOS is currently not supported!"
    #elif TARGET_OS_MAC == 1 // MacOS
        #define HARMONY_PLATFORM_MACOS
    #else
        #error "Unknown Unsupported Apple Operating System Detected"
    #endif
#elif defined(__ANDROID__) // Android
    #define HARMONY_PLATFORM_ANDROID
    #error "Android is currently not supported!"
#elif defined(__linux__) // Linux
    #define HARMONY_PLATOFORM_LINUX
#else  
    #define HARMONY_PLATFORM_UNDEFINED
    #error "Unknown Unsupported Operating System Detected"
#endif

class Platform {

public:
    static const std::string& GetHomeDirectory() {
        static std::string homepath(getenv("HOME"));
        return homepath;
    }
};