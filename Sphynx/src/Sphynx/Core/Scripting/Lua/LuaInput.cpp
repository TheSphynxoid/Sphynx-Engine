#include "pch.h"
#include "LuaInput.h"
#include "Application.h"
#include "Input.h"
#include "Core/Graphics/Window.h"
#include "LuaIncludes.h"

static const luaL_Reg InputMeta[] = {
	{NULL,NULL}
};

static const luaL_Reg InputFunc[] = {
	{NULL,NULL}
};

void Sphynx::Core::Scripting::LuaInput::CreateModule(lua_State* L)
{
	luaL_newmetatable(L, "Input");
	luaL_setfuncs(L, InputMeta, 0);
	luaL_newlibtable(L, InputFunc);
	luaL_setfuncs(L, InputMeta, 0);
	lua_setfield(L, -2, "__index");
	lua_setglobal(L, "Input");
}
