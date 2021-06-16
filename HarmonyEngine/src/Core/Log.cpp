#include "Log.h"

using namespace HarmonyEngine;

#ifdef HARMONY_DEBUG_ENABLED
Log::LogLevel Log::s_LogLevel = Log::LogLevel::LogLevelInfo;
#else
Log::LogLevel Log::s_LogLevel = Log::LogLevel::LogLevelError;
#endif