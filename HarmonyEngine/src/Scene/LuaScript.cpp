#include "LuaScript.h"

using namespace HarmonyEngine;

// ======================= Lua Types ================================
// - Nil (similar to null in java)
// - Boolean
// - Light Userdata (pointer)
// - Number (ints, floats, as 64-bit number)
// - String (remember strings are garbage collected (copy them don't just hold pointers))
// - Table (Only complex data type!!!! Similar to js objects)
// - Function (still considered a type (all functions are lambdas))
// - Userdata (Unique created types)
// - Thread (?? Something to look into ??)
// ==================================================================

// ---- Calling a function ----
// lua_pcall(LuaState, number of args, number of return values, error handler)

// Set global pops previous item off stack!

LuaScript::LuaScript() {
    L = luaL_newstate(); // Create the lua state
}

LuaScript::~LuaScript() {
    lua_close(L); // Close the lua state
}

void LuaScript::OpenScript(const std::filesystem::path& scriptPath) {
    luaL_openlibs(L);

    // Bind Native Functions

    // Load Harmony Library
    HARMONY_ASSERT_MESSAGE(HandleLua(luaL_dostring(L, GetHarmonyLibrary().c_str())), "Could not load HarmonyLibrary.lua");

    // Load the script
    // TODO: Don't read throught FileUtils
    std::string scriptContents = FileUtils::ReadFile(scriptPath);
    HandleLua(luaL_dostring(L, scriptContents.c_str())); // load the script
    
    // Check for OnUpdate function
    lua_getglobal(L, "OnUpdate");
    
    if(lua_isfunction(L, -1))
        m_ContainsOnUpdateFunction = true;
}

void LuaScript::OnCreate() {

    HARMONY_PROFILE_FUNCTION();

    lua_getglobal(L, "OnCreate");

    if(lua_isfunction(L, -1)) {
        HandleLua(lua_pcall(L, 0, 0, 0));
    }
}

void LuaScript::OnUpdate(float deltaTime) {

    HARMONY_PROFILE_FUNCTION();

    if(m_ContainsOnUpdateFunction) {
        lua_getglobal(L, "OnUpdate");

        lua_pushnumber(L, deltaTime);

        HandleLua(lua_pcall(L, 1, 0, 0));
    }
}

void LuaScript::OnDestroy() {

    HARMONY_PROFILE_FUNCTION();

    lua_getglobal(L, "OnDestroy");

    if(lua_isfunction(L, -1)) {
        HandleLua(lua_pcall(L, 0, 0, 0));
    }
}

const std::string& LuaScript::GetHarmonyLibrary() {
    HARMONY_PROFILE_FUNCTION();

    // TODO: Don't read through FileUtils
    static const std::string libraryCode = FileUtils::ReadFile("engineAssets/lua/HarmonyLibrary.lua");
    return libraryCode;
}

bool LuaScript::HandleLua(int result) {
    HARMONY_PROFILE_FUNCTION();

    if(result != LUA_OK) {
        std::string errormsg = lua_tostring(L, -1);
        Log::Error(errormsg);
        return false;
    }

    return true;
}