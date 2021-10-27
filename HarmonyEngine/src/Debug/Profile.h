#pragma once

#include "harmonypch.h"

#ifdef DEBUG

namespace HarmonyEngine {

    class ScopeTimer {
        
    public:
        ScopeTimer() = default;
        ScopeTimer(const std::string& scopeName) : m_ScopeName(scopeName), m_StartTimepoint(std::chrono::steady_clock::now()) {

        }

        ~ScopeTimer() {
            auto endTimepoint = std::chrono::steady_clock::now();
            auto highResStart = std::chrono::duration<double, std::micro>{m_StartTimepoint.time_since_epoch()};
            auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
                - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

            Log::Warn("{} {}", m_ScopeName, elapsedTime.count());
        }

    private:
        std::string m_ScopeName;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
        
    };

};

#ifndef __FUNCSIG__
    #define __FUNCSIG__ __PRETTY_FUNCTION__
#endif

#define HARMONY_PROFILE_SCOPE(scopeName) ::HarmonyEngine::ScopeTimer scopeTimer##__LINE__(scopeName);
#define HARMONY_PROFILE_FUNCTION() HARMONY_PROFILE_SCOPE(__FUNCSIG__);

#else

#define HARMONY_PROFILE_SCOPE(scopeName);

#endif