#include "pch.h"
#include "LuaInput.h"
#include "Application.h"
#include "Input.h"
#include "Core/Graphics/Window.h"
#include "LuaIncludes.h"

int GetKey(lua_State* L) {
	if (lua_gettop(L) == 1) {
		auto keycode = lua_tointeger(L, 1);
		lua_pushboolean(L, Sphynx::Input::IsKeyPressed((Sphynx::Keys)keycode));
		return 1;
	}
	return 0;
}

static const luaL_Reg InputMeta[] = {
	{"__index",NULL},
	{NULL,NULL}
};

static const luaL_Reg InputFunc[] = {
	{"GetKey",&GetKey},
	{NULL,NULL}
};

void Sphynx::Core::Scripting::LuaInput::CreateModule(lua_State* L)
{
	//TODO: Fix
	Core_Warn("Lua Input is Incomplete");
	luaL_newmetatable(L, "Input");
	luaL_setfuncs(L, InputMeta, 0);  /* add metamethods to new metatable like __index */
	luaL_newlibtable(L, InputFunc);  /* create method table */
	luaL_setfuncs(L, InputFunc, 0);  /* add Input methods to method table */
	lua_setfield(L, -2, "__index");  /* metatable.__index = method table */
	lua_setglobal(L, "Input");
}
