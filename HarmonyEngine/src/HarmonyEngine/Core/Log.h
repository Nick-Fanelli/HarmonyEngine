#pragma once

#include "harmonypch.h"

#include <spdlog/spdlog.h>

namespace HarmonyEngine {

    class Log {

    public:
        static void Initialize();

        template<typename T>
        static inline void Info(const T& message) { s_Logger->info(message); }

        template<typename T>
        static inline void Warn(const T& message) { s_Logger->warn(message); }

        template<typename T>
        static inline void Error(const T& message) { s_Logger->error(message); }

        template<typename T>
        static inline void Trace(const T& message) { s_Logger->trace(message); }
    private:
        static inline std::shared_ptr<spdlog::logger> s_Logger;
    };

}