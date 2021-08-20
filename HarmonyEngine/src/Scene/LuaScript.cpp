#include "LuaScript.h"

using namespace HarmonyEngine;

#include <Core/Input.h>

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
    // L = luaL_newstate(); // Create the lua state
}

LuaScript::~LuaScript() {
    if(L != nullptr)
        lua_close(L); // Close the lua state
    L = nullptr;
}

// Native Functions
int NativeIsKey(lua_State* L) {

    auto keyCode = lua_tonumber(L, -1); // The first argument
    lua_pushboolean(L, Input::IsKey(keyCode));

    return 1; // 1 = amount of return values
}

int NativeIsKeyDown(lua_State* L) {

    auto keyCode = lua_tonumber(L, -1);
    lua_pushboolean(L, Input::IsKeyDown(keyCode));

    return 1;
}

int NativeIsKeyUp(lua_State* L) {

    auto keyCode = lua_tonumber(L, -1);
    lua_pushboolean(L, Input::IsKeyUp(keyCode));

    return 1;
}

int NativeGetMousePosition(lua_State* L) {

    auto mousePosition = Input::GetMousePosition();

    lua_newtable(L);
    lua_pushinteger(L, mousePosition.x);
    lua_setfield(L, -2, "x");
    lua_pushinteger(L, mousePosition.y);
    lua_setfield(L, -2, "y");

    return 1;
} 

void LuaScript::LoadGlobalScript(const std::filesystem::path& scriptPath) {

    if(m_IsAssigned)
        lua_close(L);

    L = luaL_newstate();

    m_IsAssigned = true;
    m_Filepath = scriptPath;

    luaL_openlibs(L);

    // Load Harmony Library
    HARMONY_ASSERT_MESSAGE(HandleLua(luaL_dostring(L, GetHarmonyLibrary().c_str())), "Could not load HarmonyLibrary.lua");

    // Bind Native Functions
    // IsKey
    lua_pushcfunction(L, NativeIsKey);
    lua_setglobal(L, "IsKey");

    // IsKeyDown
    lua_pushcfunction(L, NativeIsKeyDown);
    lua_setglobal(L, "IsKeyDown");

    // IsKeyUp
    lua_pushcfunction(L, NativeIsKeyUp);
    lua_setglobal(L, "IsKeyUp");

    // _NativeGetMousePosition
    lua_pushcfunction(L, NativeGetMousePosition);
    lua_setglobal(L, "_NativeGetMousePosition");

    // Load the script
    std::string line;
    std::string scriptContents;

    std::ifstream file(scriptPath);
    if(file.is_open()) {
        while(std::getline(file, line)) {
            scriptContents += line + "\n";
        }
        file.close();
    }

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

    static std::string libraryCode = "";

    if(libraryCode == "") {

        std::string line;

        std::ifstream file("engineAssets/lua/HarmonyLibrary.lua");
        if(file.is_open()) {
            while(std::getline(file, line)) {
                libraryCode += line + "\n";
            }

            file.close();
        }        
    }

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