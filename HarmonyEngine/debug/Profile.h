#pragma once

#include "harmonypch.h"

#include <chrono>
#include <string>

class ScopeTimer {
    
public:
    ScopeTimer() = default;
    ScopeTimer(const std::string& scopeName);
    ~ScopeTimer();

private:
    std::string m_ScopeName;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    
};