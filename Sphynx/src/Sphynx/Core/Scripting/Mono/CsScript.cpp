#include "pch.h"
#include "CsScript.h"
#include "MonoRuntime.h"
#include "GameObject.h"
#include "Core/ThreadPool.h"

Sphynx::Mono::CsScript::CsScript(MonoObject* obj, std::string name) : ScriptObject(obj), Name(name)
{
}

void Sphynx::Mono::CsScript::Start()
{
	Sphynx::Core::ThreadPool::Submit(StartThunk);
}
