#include "pch.h"
#include "CsScript.h"
#include "MonoRuntime.h"
#include "GameObject.h"
#include "Core/ThreadPool.h"

Sphynx::Mono::CsScript::CsScript(MonoObject* obj, std::string name) : ScriptObject(obj), Name(name)
{
	IsValid = true;
}

void Sphynx::Mono::CsScript::Start()
{
	//Sphynx::Core::ThreadPool::Submit(StartThunk);
}

Sphynx::Core::Scripting::Script* Sphynx::Mono::CsScript::Copy()
{
	MonoObject* obj = mono_object_clone(ScriptObject);
	auto script = new CsScript(obj, Name);
	return script;
}
