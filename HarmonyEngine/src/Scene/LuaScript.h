#pragma once

#include "harmonypch.h"

namespace HarmonyEngine {

    class LuaScript {

    public: 
        LuaScript() = default;
        LuaScript(std::filesystem::path& filepath) : m_Filepath(filepath) {}

    private:
        std::filesystem::path m_Filepath;

    };

}