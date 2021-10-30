#pragma once

#include "harmonypch.h"

#include <ctime>

namespace HarmonyEngine {

    static const char* ANSI_RESET = "\033[0m";
    
    static const char* ANSI_RED = "\033[0;31m";
    static const char* ANSI_RED_BOLD = "\033[1;31m";

    static const char* ANSI_YELLOW = "\033[0;33m";
    static const char* ANSI_YELLOW_BOLD = "\033[1;33m";

    static const char* ANSI_GREEN = "\033[0;32m";
    static const char* ANSI_GREEN_BOLD = "\033[1;32m";

    struct TimeData {

        int Hour;
        int Minute;
        int Second;

    };

    inline std::ostream& operator<<(std::ostream& out, const TimeData& data) {
        return out << (data.Hour < 10 ? "0" : "") << data.Hour << ":" << (data.Minute < 10 ? "0" : "") << data.Minute << ":" << (data.Second < 10 ? "0" : "") << data.Second;
    }


    class Log {
    
    public:
        enum LogLevel {
            LogLevelError = 0, LogLevelWarn = 1, LogLevelInfo = 2
        };
    
    private:

        static LogLevel s_LogLevel;

    private:

        static TimeData GetCurrentTime() {
            time_t now = time(0);

            tm *ltm = localtime(&now);

            return { ltm->tm_hour, ltm->tm_min, ltm->tm_sec };
        }

    public:

        static const LogLevel& GetLogLevel() { return s_LogLevel; }
        static void SetLogLevel(const LogLevel& logLevel) { s_LogLevel = logLevel; }

        template<typename... Values>
        static void FormatTrace(const char* message, Values&&... values) {
            // if(s_LogLevel >= LogLevelInfo) {
            //     printf("%sHarmony Engine - [INFO]:%s ", ANSI_BLUE_BOLD, ANSI_BLUE);
            //     printf(message, std::forward<Values>(values)...);
            //     printf("%s\n", ANSI_RESET);
            // }
        }

        template<typename... Values>
        static void FormatInfo(const char* message, Values&&... values) {
            if(s_LogLevel >= LogLevelInfo) {
                printf("%sHarmony Engine - [SUCCESS]:%s ", ANSI_GREEN_BOLD, ANSI_GREEN);
                printf(message, std::forward<Values>(values)...);
                printf("%s\n", ANSI_RESET);
            }
        }

        template<typename... Values>
        static void FormatWarn(const char* message, Values&&... values) {
             if(s_LogLevel >= LogLevelWarn) {
                printf("%sHarmony Engine - [WARN]:%s ", ANSI_YELLOW_BOLD, ANSI_YELLOW);
                printf(message, std::forward<Values>(values)...);
                printf("%s\n", ANSI_RESET);
            }
        }

        template<typename... Values>
        static void FormatError(const char* message, Values&&... values) {
             if(s_LogLevel >= LogLevelError) {
                printf("%sHarmony Engine - [ERROR]:%s ", ANSI_RED_BOLD, ANSI_RED);
                printf(message, std::forward<Values>(values)...);
                printf("%s\n", ANSI_RESET);
            }
        }

        template<typename T>
        static void Trace(const T& message) {
            if(s_LogLevel >= LogLevelInfo) 
                std::cout << "[" << GetCurrentTime() << "] [HarmonyEngine]: " << message << std::endl;
        }

        template<typename T>
        static void Info(const T& message) {
            if(s_LogLevel >= LogLevelInfo)
                std::cout << ANSI_GREEN_BOLD << "Harmony Engine - [SUCCESS]: " << ANSI_GREEN << message << ANSI_RESET << std::endl;
        }

        template<typename T>
        static void Warn(const T& message) {
            if(s_LogLevel >= LogLevelWarn)
                std::cout << ANSI_YELLOW_BOLD << "Harmony Engine - [WARNING]: " << ANSI_YELLOW << message << ANSI_RESET << std::endl;
        }

        template<typename T>
        static void Error(const T& message) {
            if(s_LogLevel >= LogLevelError)
                std::cout << ANSI_RED_BOLD << "Harmony Engine - [ERROR]: " << ANSI_RED << message << ANSI_RESET << std::endl;
        }
    };

}