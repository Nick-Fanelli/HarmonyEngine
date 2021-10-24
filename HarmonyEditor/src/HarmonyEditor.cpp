#include <HarmonyEngine.h>

using namespace HarmonyEngine;

int main() {

    Log::Initialize();

    Log::Info("Hello World {}", "Test");
    Log::Warn("Hello World");
    Log::Error("Hello World");
    Log::Trace("Hello World");

    return 0;
}