#include "pch.h"
#include "Lua.h"
#include <lua.hpp>
#include "Application.h"
#include "LuaWindow.h"

using namespace Sphynx::Core::Scripting;

bool PanicTerminate = true;
bool PanicDebug = true;

//g_ = getter
//s_ = setter

int print(lua_State* L) {
	int params = lua_gettop(L);
	for (int i = 1; i < params + 1; i++) {
		size_t l;
		const char* str = luaL_tolstring(L, i, &l);
		Core_Info(str);
	}
	return 0;
}

int lua_panic(lua_State* L) {
	if (PanicTerminate) {
		if(PanicDebug)
			std::terminate();
		else
			//Kill the Process
			_exit(-1);
	}
	else {
		Core_Error("Lua Error.");
	}
	return 0;
}
int dump(lua_State* L) {
	Lua(L).DumpStack();
	return 0;
}
int loadfile(lua_State* L) {
	if (!lua_gettop(L)) {
		Core_Error("No Path Was Given");
		return 0;
	}
	if (lua_isstring(L, -1)) {
		Lua(L).LoadFile(lua_tostring(L, -1));
		return 1;
	}
	return 0;
}
int loadstring(lua_State* L) {
	if (!lua_gettop(L)) {
		Core_Error("No Path Was Given");
		return 0;
	}
	if (lua_isstring(L, -1)) {
		Lua(L).LoadString(lua_tostring(L, -1));
		return 1;
	}
	return 0;
}
int executefile(lua_State* L) {
	if (!lua_gettop(L)) {
		Core_Error("No Path Was Given");
		return 0;
	}
	if (lua_isstring(L, -1)) {
		Lua(L).ExecuteFile(lua_tostring(L, -1));
		return 1;
	}
	return 0;
}
int executestring(lua_State* L) {
	if (!lua_gettop(L)) {
		Core_Error("No Path Was Given");
		return 0;
	}
	if (lua_isstring(L, -1)) {
		Lua(L).ExecuteString(lua_tostring(L, -1));
		return 1;
	}
	return 0;
}
int g_deltaTime(lua_State* L) {
	auto dt = Sphynx::Time::GetDeltaTime();
	lua_pushnumber(L, dt);
	return 1;
}

Sphynx::Core::Scripting::Lua::Lua(Sphynx::Application* app)
{
	State = luaL_newstate();
	LuaWindow::CreateModule(State, app);
	luaL_openlibs(State);
	lua_atpanic(State, &lua_panic);
	lua_getglobal(State, "print");
	lua_setglobal(State, "printf"); //So we don't lose the original print
	lua_register(State, "print", &print);
	lua_register(State, "LoadFile", &loadfile);
	lua_register(State, "ExecuteFile", &executefile);
	lua_register(State, "LoadString", &loadstring);
	lua_register(State, "ExecuteString", &executestring);
	lua_register(State, "DumpStack", &dump);
	lua_register(State, "GetDeltaTime", &g_deltaTime);
}

Sphynx::Core::Scripting::Lua::~Lua()
{
	//lua_close(State);
}

void Sphynx::Core::Scripting::Lua::Close()
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
	Sleep(1000);
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

void Sphynx::Core::Scripting::Lua::LoadFile(const char* path)
{
	if (luaL_loadfile(State, path) == LUA_ERRFILE) {
		Core_Error("stack = {0}", lua_gettop(State));
		Core_Error("error = {0}", LUA_ERRFILE);
		Core_Error("message = {0}", lua_tostring(State, -1));
	}
}

void Sphynx::Core::Scripting::Lua::LoadString(const char* code)
{

}

void Sphynx::Core::Scripting::Lua::AddScript(const char* str, bool IsFile)
{
}

void Sphynx::Core::Scripting::Lua::RaiseError(const char* fmt, ...)
{
	luaL_error(State, fmt);
}

void Sphynx::Core::Scripting::Lua::RegisterFunction(Lua_CFunc func, const char* name)
{
	lua_register(State, name, (lua_CFunction)func);
}

void Sphynx::Core::Scripting::Lua::DumpStack()
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

void Sphynx::Core::Scripting::Lua::DumpStack(lua_State* State)
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

void Sphynx::Core::Scripting::Lua::SetAtPanicKillSwitch(bool b, bool debug)
{
	PanicDebug = debug; 
	PanicTerminate = b;
}

const char* Sphynx::Core::Scripting::Lua::GetErrorString()
{
	if (lua_isstring(State, -1)) {
		Core_Error("message = {0}", lua_tostring(State, -1));
	}
	return nullptr;
}

Sphynx::Core::Scripting::Lua::operator lua_State* ()
{
	return State;
}
