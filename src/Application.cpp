#include "harmonypch.h"

#include "Core/Log.h"

using namespace HarmonyEngine;

int main() {

    Log::SetLogLevel(Log::LogLevel::LogLevelInfo);

    Log::Info("This is some information");
    Log::Success("This is a success");
    Log::Warn("This is a warning");
    Log::Error("This is an error");

}