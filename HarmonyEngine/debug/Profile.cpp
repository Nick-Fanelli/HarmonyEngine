#include "Profile.h"

using namespace HarmonyEngine;

ScopeTimer::ScopeTimer(const std::string& scopeName) : m_ScopeName(scopeName), m_StartTimepoint(std::chrono::steady_clock::now()) {
}

ScopeTimer::~ScopeTimer() {
    auto endTimepoint = std::chrono::steady_clock::now();
    auto highResStart = std::chrono::duration<double, std::micro>{m_StartTimepoint.time_since_epoch()};
    auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
        - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

    Log::Warn("{} {}", m_ScopeName, elapsedTime.count());

}  