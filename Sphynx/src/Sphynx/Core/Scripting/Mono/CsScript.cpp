#include "pch.h"
#include "CsScript.h"
#include "MonoRuntime.h"
#include "GameObject.h"
#include "Core/ThreadPool.h"

void Sphynx::Mono::CsScript::Init()
{
	AwakeVirtMethod = mono_class_get_method_from_name(ComponentClass, "Awake", 0);
	StartVirtMethod = mono_class_get_method_from_name(ComponentClass, "Start", 0);
	UpdateVirtMethod = mono_class_get_method_from_name(ComponentClass, "Update", 0);
	FixedUpdateVirtMethod = mono_class_get_method_from_name(ComponentClass, "FixedUpdate", 0);
	OnDestroyVirtMethod = mono_class_get_method_from_name(ComponentClass, "OnDestroy", 0);
	//NativeField = mono_class_get_field_from_name(ComponentClass, "Native");
	ID = mono_class_get_field_from_name(ComponentClass, "ID");
}

void Sphynx::Mono::CsScript::SetInternalID(size_t id)
{
	mono_field_set_value(ScriptObject, ID, &id);
}

Sphynx::Mono::CsScript::CsScript(MonoObject* obj, MonoClass* objClass, std::string name, const EntityID& id) : ScriptObject(obj), ScriptClass(objClass), Name(name)
{
	AwakeThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, AwakeVirtMethod));
	StartThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, StartVirtMethod));
	UpdateThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, UpdateVirtMethod));
	FixedUpdateThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, FixedUpdateVirtMethod));
	OnDestroyThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(ScriptObject, OnDestroyVirtMethod));
	
	SetInternalID(id);
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
	auto script = new CsScript(obj, ScriptClass, Name, (EntityID)0);
	return script;
}
