#include "pch.h"
#include "ScriptingEngine.h"

void CorePrint(lua_State* L) {
	Core_Info("Called From Lua");
}

void Sphynx::Core::Scripting::ScriptingEngine::Start(Application* app)
{
	LuaState = new Lua();
}

void Sphynx::Core::Scripting::ScriptingEngine::Update()
{
}

Sphynx::Core::Scripting::Lua* Sphynx::Core::Scripting::ScriptingEngine::CreateLuaThread()
{
	auto s = LuaState;
	LuaState = s->CreateThread();
	return s;
}
