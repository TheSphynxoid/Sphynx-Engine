#include "pch.h"
#include "LuaWindow.h"
#include "Application.h"
#include "Core/Graphics/Platform/GLWindow.h"
#include "Core/Scripting/Lua/Lua.h"
#include <lua.hpp>

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Core::Scripting;

static Sphynx::Application* App;

typedef struct L_Window {
	IWindow* IWin;
}L_Window;

L_Window* createLuaWindow(lua_State* L, IWindow* win) {
	L_Window* w = reinterpret_cast<L_Window*>(lua_newuserdata(L, sizeof(L_Window)));
	w->IWin = win;
	luaL_setmetatable(L, "Window");
	return w;
}

IWindow* ToWindow(lua_State* L) {
	L_Window* w = reinterpret_cast<L_Window*>(luaL_checkudata(L, 1, "Window"));
	return w->IWin;
}
//TODO:reduce the size of this function.
int createWindow(lua_State* L) {
	int params = lua_gettop(L);
	if (params < 4)return 0;
	if (lua_type(L, 1) == LUA_TSTRING) {
		size_t size;
		const char* str = lua_tolstring(L, 1, &size);
		if (strcmp(str,"GL") == 0) {
			int width, height;
			if (lua_type(L, 2) == LUA_TNUMBER && lua_type(L, 3) == LUA_TNUMBER && lua_type(L,4) == LUA_TSTRING) {
				width = lua_tonumberx(L, 2, 0);
				height = lua_tonumberx(L, 3, 0);
				Bounds b = { width, height };
				size_t tsize;
				const char* title = lua_tolstring(L, 4, &tsize);
				IWindow* win = new Sphynx::Core::GLWindow(App, b, title, App->GetMainWindow());
				auto lwin = createLuaWindow(L, win);
				return 1;
			}
		}
		else {
			Core_Error(str, " Invalid Window API");
			return 0;
		}
		return 0;
	}
	return 0;
}

int w_gc(lua_State* L) {
	L_Window* w = reinterpret_cast<L_Window*>(luaL_checkudata(L, 1, "Window"));
	w->IWin->Close();
	w = nullptr;
	lua_getmetatable(L, -1);
	lua_pushboolean(L, false);
	lua_setfield(L, -2, "IsOpen");
	lua_setmetatable(L, -1);
	return 0;
}
int WindowClose(lua_State* L) {
	lua_getmetatable(L, 1);
	int type = lua_getfield(L, -1, "IsOpen");
	if (lua_toboolean(L, -2)) {
		w_gc(L);
		return 0;
	}
	else {
		Core_Error("Attempting to Close a Closed window");
		luaL_pushfail(L);
		return 1;
	}
}
int WindowChangeTitle(lua_State* L) {
	if (lua_gettop(L) != 2 || !lua_isstring(L,2)) {
		luaL_pushfail(L);
		return 1;
	}
	else {
		const char* s = lua_tostring(L, 2);
		IWindow* win = ToWindow(L);
		win->ChangeTitle(s);
		return 0;
	}
}
int WindowResize(lua_State* L) {
	if (lua_gettop(L) != 3 || !(lua_isnumber(L, 2) && lua_isnumber(L,3))) {
		luaL_pushfail(L);
		return 1;
	}
	else {
		int width = lua_tonumber(L, 2);
		int height = lua_tonumber(L, 3);
		IWindow* win = ToWindow(L);
		win->Resize(width, height);
		return 0;
	}
}
int WindowStart(lua_State* L) {
	IWindow* win = ToWindow(L);
	win->Start();
	return 0;
}
int WindowSetVSync(lua_State* L) {
	if (lua_gettop(L) != 2)return 0;
	IWindow* win = ToWindow(L);
	if (lua_isboolean(L, -1)) {
		win->SetVsync(lua_toboolean(L, -1));
	}
	return 0;
}
int WindowGetVSync(lua_State* L) {
	IWindow* win = ToWindow(L);
	lua_pushboolean(L, win->IsVsyncEnabled());
	return 1;
}
int WindowUpdate(lua_State* L) {
	IWindow* win = ToWindow(L);
	win->Update();
	return 0;
}
int WindowIsOpen(lua_State* L) {
	IWindow* win = ToWindow(L);
	lua_pushboolean(L, win->IsAlive());
	return 1;
}
static const luaL_Reg libmeth[] = {
	{"ChangeTitle"	,WindowChangeTitle},
	{"Close"		,w_gc},
	{"Start"		,WindowStart},
	{"Update"		,WindowUpdate},
	{"SetVSync"		,WindowSetVSync},
	{"GetVSync"		,WindowGetVSync},
	{"IsAlive"		,WindowIsOpen},
	{NULL,NULL}
};
static const luaL_Reg metameth[] = {
	{"__index"	,NULL},
	{"__gc"		,w_gc},
	{"__close"	,w_gc},
	{NULL,NULL}
};
void Sphynx::Core::Scripting::LuaWindow::CreateWindowMeta(lua_State* L)
{
	luaL_newmetatable(L, "Window");
	luaL_setfuncs(L, metameth, 0);  /* add metamethods to new metatable like __index */
	luaL_newlibtable(L, libmeth);  /* create method table */
	luaL_setfuncs(L, libmeth, 0);  /* add Window methods to method table */
	lua_setfield(L, -2, "__index");  /* metatable.__index = method table */
	lua_setglobal(L, "Window");
}

int createBounds(lua_State* L) {
	Bounds* b = (Bounds*)lua_newuserdatauv(L, sizeof(Bounds), 0);
	if (lua_gettop(L) < 2 || !(lua_isnumber(L, 1) && lua_isnumber(L, 2))) {
		b->Height = 0;
		b->Width = 0;
	}
	else {
		b->Width = lua_tonumber(L, 1);
		b->Height = lua_tonumber(L, 2);
	}
	luaL_getmetatable(L, "Bounds");
	lua_setmetatable(L, -2);
	return 1;
}
static const luaL_Reg boundsmmeth[]{
	{"__index",NULL},
	{"__call", createBounds},
	{NULL,NULL}
};
static const luaL_Reg boundsM[]{
	//{"new", createBounds},
	{NULL,NULL}
};

void Sphynx::Core::Scripting::LuaWindow::CreateBoundsMeta(lua_State* L)
{
	luaL_newmetatable(L, "Bounds");
	luaL_setfuncs(L, boundsmmeth, 0);
	luaL_newlibtable(L, boundsM);  /* create method table */
	luaL_setfuncs(L, boundsM, 0);
	lua_setfield(L, -2, "__index");
	lua_setglobal(L, "Bounds");
	//lua_pop(L, 1);
}

void Sphynx::Core::Scripting::LuaWindow::CreateModule(lua_State* L, Sphynx::Application* app)
{
	App = app;
	lua_register(L, "CreateWindow", &createWindow);
	CreateWindowMeta(L);
	CreateBoundsMeta(L);
}
