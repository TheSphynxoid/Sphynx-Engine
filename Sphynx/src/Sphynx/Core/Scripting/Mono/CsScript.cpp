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
			auto lock = Sphynx::Core::ThreadPool::GetLock();
			MonoRuntime::ReloadGameAssembly();
		}
	}
}

Sphynx::Mono::CsScript::CsScript(MonoObject* obj, MonoClass* objClass, std::string name) : ScriptObject(obj), ScriptClass(objClass), Name(name)
{
	AwakeThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, AwakeVirtMethod));
	StartThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, StartVirtMethod));
	UpdateThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, UpdateVirtMethod));
	FixedUpdateThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, FixedUpdateVirtMethod));
	OnDestroyThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, OnDestroyVirtMethod));
}

void Sphynx::Mono::CsScript::Awake()
{
	Sphynx::Core::ThreadPool::Submit([*this]() {
		MonoException* ex;
		AwakeThunk(ScriptObject, &ex);
		HandleException(ex);
		});
}

void Sphynx::Mono::CsScript::Start()
{
	Sphynx::Core::ThreadPool::Submit([*this]() { 
			MonoException* ex;
			StartThunk(ScriptObject, &ex);
			HandleException(ex);
		});
}

void Sphynx::Mono::CsScript::Update()
{
	Sphynx::Core::ThreadPool::Submit([*this]() {
		MonoException* ex;
		UpdateThunk(ScriptObject, &ex);
		HandleException(ex);
		});
}

void Sphynx::Mono::CsScript::FixedUpdate()
{
	Sphynx::Core::ThreadPool::Submit([*this]() {
		MonoException* ex;
		FixedUpdateThunk(ScriptObject, &ex);
		HandleException(ex);
		});
}

void Sphynx::Mono::CsScript::OnDestroy()
{
	Sphynx::Core::ThreadPool::Submit([*this]() {
		MonoException* ex;
		OnDestroyThunk(ScriptObject, &ex);
		HandleException(ex);
		});
}

Sphynx::Core::Scripting::Script* Sphynx::Mono::CsScript::Copy()
{
	MonoObject* obj = mono_object_clone(ScriptObject);
	auto script = new CsScript(obj, ScriptClass, Name);
	return script;
}
