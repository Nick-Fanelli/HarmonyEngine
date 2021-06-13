#include "Log.h"

using namespace HarmonyEngine;

Log::LogLevel Log::s_LogLevel = Log::LogLevel::LogLevelInfo;

void Log::FormatInfo(const char* format, ...) {
    va_list args;
    va_start(args, format);

    if(s_LogLevel >= LogLevelInfo) {
        printf("%sHarmonyEngine - [INFO]:%s ", ANSI_BLUE_BOLD, ANSI_BLUE);
        vfprintf(stdout, format, args);
        printf("%s\n", ANSI_RESET);
    }
}

void Log::FormatSuccess(const char* format, ...) {
    va_list args;
    va_start(args, format);

    if(s_LogLevel >= LogLevelInfo) {
        printf("%sHarmony Engine - [SUCCESS]:%s ", ANSI_GREEN_BOLD, ANSI_GREEN);
        vfprintf(stdout, format, args);
        printf("%s\n", ANSI_RESET);
    }
}

void Log::FormatWarn(const char* format, ...) {
    va_list args;
    va_start(args, format);

    if(s_LogLevel >= LogLevelWarn) {
        printf("%sHarmony Engine - [WARNING]:%s ", ANSI_YELLOW_BOLD, ANSI_YELLOW);
        vfprintf(stdout, format, args);
        printf("%s\n", ANSI_RESET);
    }

    va_end(args);
}

void Log::FormatError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    if(s_LogLevel >= LogLevelError) {
        printf("%sHarmony Engine - [ERROR]:%s ", ANSI_RED_BOLD, ANSI_RED);
        vfprintf(stdout, format, args);
        printf("%s\n", ANSI_RESET);
    }

    va_end(args);
}