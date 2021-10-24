#pragma once

#include <iostream>

namespace HarmonyEngine {

    class Log {

    public:
        template<typename T>
        static void Info(const T& message) { std::cout << message << std::endl; }

    };

}