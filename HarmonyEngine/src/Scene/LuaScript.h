#pragma once

#include "harmonypch.h"

// Lua Implementation
extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

namespace HarmonyEngine {

    class LuaScript {

    public:
        LuaScript();
        ~LuaScript();

    public:
        void OpenScript(const std::filesystem::path& scriptPath);

        void OnCreate();
        void OnUpdate(float deltaTime);
        void OnDestroy();

    private:    
        static const std::string& GetHarmonyLibrary();

        bool HandleLua(int result);
        
    private:
        lua_State* L = nullptr;
        bool m_ContainsOnUpdateFunction = false;
    
    };

}