#include "pch.h"
#include "ScriptingEngine.h"

void Sphynx::Core::Scripting::ScriptingEngine::Start(Application* app)
{
	LuaState = new Lua(app);
}

void Sphynx::Core::Scripting::ScriptingEngine::Update()
{
}

Sphynx::Core::Scripting::Lua* Sphynx::Core::Scripting::ScriptingEngine::CreateLuaThread()
{
	auto s = LuaState->CreateThread();
	return s;
}
