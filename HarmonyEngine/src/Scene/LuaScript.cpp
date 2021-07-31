#include "LuaScript.h"

using namespace HarmonyEngine;

LuaScript::LuaScript(const std::filesystem::path& filepath) : m_Filepath(filepath) {

    L = luaL_newstate();
    luaL_openlibs(L);

    if(!CheckLua(luaL_dofile(L, filepath.c_str()))) {
        L = nullptr;
        return;
    }
}

LuaScript::~LuaScript() {
    if(L != nullptr) {
        lua_close(L);
        L = nullptr;
    }
}

bool LuaScript::CheckLua(int result) {
    HARMONY_PROFILE_FUNCTION();

    if(result != LUA_OK) {
        std::string errormsg = lua_tostring(L, -1);
        Log::Error(errormsg);
        return false;
    }

    return true;
}

void LuaScript::CallGlobalFunction(const std::string& functionName) {
    HARMONY_PROFILE_FUNCTION();

    lua_getglobal(L, functionName.c_str());
    if(lua_isfunction(L, -1)) {
        CheckLua(lua_pcall(L, 0, 0, 0));
    }
}