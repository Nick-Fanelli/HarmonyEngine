#include "LuaScript.h"

using namespace HarmonyEngine;

LuaScript::LuaScript(const std::filesystem::path& filepath) : m_Filepath(filepath) {

    if(L != nullptr)
        return;

    L = luaL_newstate();
    luaL_openlibs(L);

    if(filepath.empty() || !std::filesystem::exists(filepath)) {
        lua_close(L);
        L = nullptr;
        return;
    }

    if(!CheckLua(luaL_dofile(L, filepath.c_str()))) {
        L = nullptr;
        HARMONY_ASSERT_MESSAGE(false, "Lua Syntax Error!"); // TODO: Remove and replace with better system!
    }
}

LuaScript::~LuaScript() {
    if(L != nullptr) {
        lua_close(L);
    }
}

bool LuaScript::CheckLua(int result) {
    HARMONY_PROFILE_FUNCTION();

    if(L == nullptr)
        return false;

    if(result != LUA_OK) {
        std::string errormsg = lua_tostring(L, -1);
        Log::Error(errormsg);
        return false;
    }

    return true;
}

void LuaScript::CallGlobalFunction(const std::string& functionName) {
    HARMONY_PROFILE_FUNCTION();

    if(L == nullptr)   
        return;

    lua_getglobal(L, functionName.c_str());
    if(lua_isfunction(L, -1)) {
        CheckLua(lua_pcall(L, 0, 0, 0));
    }
}