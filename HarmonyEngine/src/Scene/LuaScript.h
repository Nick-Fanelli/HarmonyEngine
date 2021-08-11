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
        LuaScript() {}
        LuaScript(const std::filesystem::path& filepath);
        ~LuaScript();

        void CallGlobalFunction(const std::string& functionName);

        template<typename T>
        void CallGlobalFunction(const std::string& functionName, const T& arg) {
            HARMONY_PROFILE_FUNCTION();

            lua_getglobal(L, functionName.c_str());
            if(lua_isfunction(L, -1)) {
                LuaPush(arg);
                CheckLua(lua_pcall(L, 1, 0, 0));
            }
        }

        template<typename T>
        inline T GetGlobalVariable(const std::string& variableName) {
            return -1;
        }

        template<>
        inline float GetGlobalVariable(const std::string& variableName) {
            HARMONY_PROFILE_FUNCTION();

            lua_getglobal(L, variableName.c_str());
            if(lua_isnumber(L, -1)) {
                float variable = (float) lua_tonumber(L, -1);
                return variable;
            }

            Log::FormatWarn("Could not find global float: %s", variableName.c_str());

            return -1.0f;
        }

        template<>
        inline std::string GetGlobalVariable(const std::string& variableName) {
            HARMONY_PROFILE_FUNCTION();

            lua_getglobal(L, variableName.c_str());
            if(lua_isstring(L, -1)) {
                std::string variable = lua_tostring(L, -1);
                return variable;
            }

            Log::FormatWarn("Could not find global string: %s", variableName.c_str());

            return "null";
        }

        operator bool() const { return L != nullptr; }
        bool IsAssigned() const { return L != nullptr; }

        const std::filesystem::path& GetFilepath() const { return m_Filepath; }

    private:
        std::filesystem::path m_Filepath;
        lua_State* L = nullptr;

        bool CheckLua(int result);

        template<typename T>
        void LuaPush(const T& data) {
            Log::Warn("Lua push not implemented");
        }   

        template<>
        inline void LuaPush(const float& data) {
            HARMONY_PROFILE_FUNCTION();
            
            lua_pushnumber(L, data);
        }
    };

}