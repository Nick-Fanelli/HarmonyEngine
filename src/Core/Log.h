#pragma once

#include "harmonypch.h"

namespace HarmonyEngine::Log {

    static const char* ANSI_RESET = "\033[0m";
    
    static const char* ANSI_RED = "\033[0;31m";
    static const char* ANSI_RED_BOLD = "\033[1;31m";

    static const char* ANSI_YELLOW = "\033[0;33m";
    static const char* ANSI_YELLOW_BOLD = "\033[1;33m";

    static const char* ANSI_BLUE_BOLD = "\033[38;5;153;1m";
    static const char* ANSI_BLUE = "\033[0;38;5;153m";

    static const char* ANSI_GREEN = "\033[0;32m";
    static const char* ANSI_GREEN_BOLD = "\033[1;32m";

    enum LogLevel {
        LogLevelError = 0, LogLevelWarn = 1, LogLevelInfo = 2
    };

    static LogLevel s_LogLevel = LogLevelInfo;

    const LogLevel& GetLogLevel() { return s_LogLevel; }
    void SetLogLevel(const LogLevel& logLevel) { s_LogLevel = logLevel; }

    template<typename T>
    void Info(const T& message) {
        if(s_LogLevel >= LogLevelInfo)
            std::cout << ANSI_BLUE_BOLD << "Harmony Engine - [INFO]: " << ANSI_BLUE << message << ANSI_RESET << std::endl;
    }

    template<typename T>
    void Success(const T& message) {
        if(s_LogLevel >= LogLevelInfo)
            std::cout << ANSI_GREEN_BOLD << "Harmony Engine - [SUCCESS]: " << ANSI_GREEN << message << ANSI_RESET << std::endl;
    }

    template<typename T>
    void Warn(const T& message) {
        if(s_LogLevel >= LogLevelWarn)
            std::cout << ANSI_YELLOW_BOLD << "Harmony Engine - [WARNING]: " << ANSI_YELLOW << message << ANSI_RESET << std::endl;
    }

    template<typename T>
    void Error(const T& message) {
        if(s_LogLevel >= LogLevelError)
            std::cout << ANSI_RED_BOLD << "Harmony Engine - [ERROR]: " << ANSI_RED << message << ANSI_RESET << std::endl;
    }

}