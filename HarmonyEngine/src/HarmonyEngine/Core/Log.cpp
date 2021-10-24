#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

using namespace HarmonyEngine;

void Log::Initialize() {
    spdlog::set_pattern("%^[%T] [%n]: %v%$");
    s_Logger = spdlog::stdout_color_mt("HarmonyEngine");
    s_Logger->set_level(spdlog::level::trace);
}