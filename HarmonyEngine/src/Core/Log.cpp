#include "Log.h"

using namespace HarmonyEngine;

#if HARMONY_DEBUG
Log::LogLevel Log::s_LogLevel = Log::LogLevel::LogLevelInfo;
#else
Log::LogLevel Log::s_LogLevel = Log::LogLevel::LogLevelError;
#endif