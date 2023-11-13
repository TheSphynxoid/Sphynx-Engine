#include "pch.h"
#include "CsScript.h"
#include "MonoRuntime.h"
#include "GameObject.h"
#include "Core/ThreadPool.h"

void Sphynx::Mono::CsScript::HandleException(MonoException* ex)
{
	if (ex != nullptr) {
		mono_print_unhandled_exception((MonoObject*)ex);
		mono_raise_exception(ex);
		//Making this what i think is thread safe.
		{
			Sphynx::Core::ThreadPool::GetLock();
			CsScript::Runtime->ReloadGameAssembly();
		}
	}
}

Sphynx::Mono::CsScript::CsScript(MonoObject* obj, std::string name) : ScriptObject(obj), Name(name)
{
	IsValid = true;
	StartThunk = (StartFunc)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, StartVirtMethod));
}

void Sphynx::Mono::CsScript::Start()
{
	Sphynx::Core::ThreadPool::Submit([*this]() { 
			MonoException* ex;
			StartThunk(ScriptObject, &ex);
			HandleException(ex);
		});
}

Sphynx::Core::Scripting::Script* Sphynx::Mono::CsScript::Copy()
{
	MonoObject* obj = mono_object_clone(ScriptObject);
	auto script = new CsScript(obj, Name);
	return script;
}
