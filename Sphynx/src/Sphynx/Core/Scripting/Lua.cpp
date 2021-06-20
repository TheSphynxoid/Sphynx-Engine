#include "pch.h"
#include "Lua.h"
#include <lua.hpp>


Sphynx::Core::Scripting::Lua::Lua()
{
	State = luaL_newstate();
	luaL_openlibs(State);
}

Sphynx::Core::Scripting::Lua::~Lua()
{
	lua_close(State);
}

Sphynx::Core::Scripting::Lua* Sphynx::Core::Scripting::Lua::CreateThread()
{
	auto l = lua_newthread(State);
	return new Lua(l);
}

void Sphynx::Core::Scripting::Lua::ExecuteFile(const char* path)
{
	if (luaL_loadfile(State, path)) {
		Core_Error("Could Not Open Lua File {0}", path);
		return;
	}
	int error = lua_pcall(State, 0, 0, 0);
	if (error) {
		Core_Error("stack = {0}", lua_gettop(State));
		Core_Error("error = {0}", error);
		Core_Error("message = {0}", lua_tostring(State, -1));
		lua_pop(State, 1);
	}
}

void Sphynx::Core::Scripting::Lua::ExecuteString(const char* code)
{
	luaL_loadstring(State, code);
	int error = lua_pcall(State, 0, 0, 0);
	if (error) {
		Core_Error("stack = {0}", lua_gettop(State));
		Core_Error("error = {0}", error);
		Core_Error("message = {0}", lua_tostring(State, -1));
		lua_pop(State, 1);
	}
}

void Sphynx::Core::Scripting::Lua::RaiseError(const char* fmt, ...)
{
	luaL_error(State, fmt);
}

void Sphynx::Core::Scripting::Lua::RegisterFunction(Lua_CFunc func, const char* name)
{
	lua_register(State, name, (lua_CFunction)func);
}

void Sphynx::Core::Scripting::Lua::PrintStack()
{
	int i;
	int top = lua_gettop(State);
	Core_Info("--Dumping Stack--");
	Core_Info("Stack Size : {0}", top);
	for (i = top; i >= 1; i--) {
		int t = lua_type(State, i);
		switch (t)
		{
		case LUA_TSTRING:
			Core_Info("Index:{0}({1}) = `{2}'", i, i - (top + 1), lua_tostring(State, i));
			break;

		case LUA_TBOOLEAN:
			Core_Info("Index:{0}({1}) = `{2}'", i, i - (top + 1), lua_toboolean(State, i) ? "true" : "false");
			break;

		case LUA_TNUMBER:
			Core_Info("Index:{0}({1}) = `{2}'", i, i - (top + 1), lua_tonumber(State, i));
			break;

		default:
			Core_Info("Index:{0}({1}) = `{2}'", i, i - (top + 1), lua_typename(State, t));
			break;
		}
	}
	Core_Info("--End Stack--");
}

const char* Sphynx::Core::Scripting::Lua::GetErrorString()
{

	return nullptr;
}
