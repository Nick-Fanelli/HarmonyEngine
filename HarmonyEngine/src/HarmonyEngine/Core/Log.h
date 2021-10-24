#pragma once

#include "harmonypch.h"

#include <spdlog/spdlog.h>

namespace HarmonyEngine {

    class Log {

    public:
        static void Initialize();

        template<typename T>
        static inline void Info(const T& message) { s_Logger->info(message); }

        template<typename... Args>
        static inline void Info(Args&&... args) { s_Logger->info(std::forward<Args>(args)...); }

        template<typename T>
        static inline void Warn(const T& message) { s_Logger->warn(message); }

        template<typename... Args>
        static inline void Warn(Args&&... args) { s_Logger->warn(std::forward<Args>(args)...); }

        template<typename T>
        static inline void Error(const T& message) { s_Logger->error(message); }
        
        template<typename... Args>
        static inline void Error(Args&&... args) { s_Logger->error(std::forward<Args>(args)...); }

        template<typename T>
        static inline void Trace(const T& message) { s_Logger->trace(message); }
        
        template<typename... Args>
        static inline void Trace(Args&&... args) { s_Logger->trace(std::forward<Args>(args)...); }

    private:
        static inline std::shared_ptr<spdlog::logger> s_Logger;
    };

}