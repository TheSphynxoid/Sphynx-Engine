#include "pch.h"
#include "GameObjectWrapper.h"
#include "../MonoRuntime.h"

void Sphynx::Mono::GameObjectWrapper::Init()
{
	auto addcomp = mono_class_get_method_from_name(GameObjectClass, "InternalAddComp", 2);
	AddComp = (AddCompThunk)mono_method_get_unmanaged_thunk(addcomp);
	NameProp = mono_class_get_property_from_name(GameObjectClass, "Name");
	TransformProp = mono_class_get_property_from_name(GameObjectClass, "transform");
	IDField = mono_class_get_field_from_name(GameObjectClass, "id");
	NativePtr = mono_class_get_field_from_name(GameObjectClass, "NativePtr");

	//Methods
	AwakeMethod = mono_class_get_method_from_name(GameObjectClass, "Awake", 0);
	StartMethod = mono_class_get_method_from_name(GameObjectClass, "Start", 0);
	UpdateMethod = mono_class_get_method_from_name(GameObjectClass, "Update", 0);
	FixedUpdateMethod = mono_class_get_method_from_name(GameObjectClass, "FixedUpdate", 0);
	OnDestroyMethod = mono_class_get_method_from_name(GameObjectClass, "OnDestroy", 0);
}

Sphynx::Mono::GameObjectWrapper::GameObjectWrapper(GameObject* go)
{
	gameObject = go;
	Managedobj = MonoRuntime::CreateObject(GameObjectClass);
	size_t ID = gameObject->GetID();
	mono_field_set_value(Managedobj, IDField, (void**)&ID);
	auto name = mono_string_new(MonoRuntime::GetAppdomain(), gameObject->GetName());
	mono_property_set_value(NameProp, Managedobj, (void**)&name, nullptr);
	mono_field_set_value(Managedobj, NativePtr, &gameObject);
	mono_runtime_object_init(Managedobj);
	transformWrapper = TransformWrapper(Managedobj, mono_property_get_value(TransformProp, Managedobj, nullptr, nullptr));

	AwakeThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(Managedobj, AwakeMethod));
	StartThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(Managedobj, StartMethod));
	UpdateThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(Managedobj, UpdateMethod));
	FixedUpdateThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(Managedobj, FixedUpdateMethod));
	OnDestroyThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(Managedobj, OnDestroyMethod));
}

Sphynx::Mono::GameObjectWrapper::GameObjectWrapper(MonoObject* obj)
{
	if (mono_object_get_class(obj) != GameObjectClass) {
		Core_Error("Invalid MonoObject. Is not GameObject class");
		return;
	}
	Managedobj = obj;
	mono_field_get_value(obj, NativePtr, &gameObject);

	AwakeThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(Managedobj, AwakeMethod));
	StartThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(Managedobj, StartMethod));
	UpdateThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(Managedobj, UpdateMethod));
	FixedUpdateThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(Managedobj, FixedUpdateMethod));
	OnDestroyThunk = (UnmanagedThunk)mono_method_get_unmanaged_thunk(mono_object_get_virtual_method(Managedobj, OnDestroyMethod));
}

Sphynx::Mono::GameObjectWrapper::~GameObjectWrapper()
{
	for (auto& script : Scripts) {
		script->OnDestroy();
	}
}

void Sphynx::Mono::GameObjectWrapper::AddComponent(CsScript* script)
{
	Scripts.push_back(script);
	auto ScriptReflection = mono_type_get_object(MonoRuntime::GetAppdomain(), mono_class_get_type((MonoClass*)script->GetNativeClass()));
	MonoException* ex;
	AddComp(this->Managedobj, (MonoObject*)script->GetNativeObject(), ScriptReflection, &ex);
	if (ex != nullptr) {
		mono_print_unhandled_exception((MonoObject*)ex);
		mono_raise_exception(ex);
		//
		throw;
	}
}

void Sphynx::Mono::GameObjectWrapper::Start()
{
	for (auto s : Scripts) {
		s->Start();
	}
}

void Sphynx::Mono::GameObjectWrapper::Update()
{
	for (auto s : Scripts) {
		s->Update();
	}
}

void Sphynx::Mono::GameObjectWrapper::FixedUpdate()
{
	for (auto script : Scripts) {
		script->FixedUpdate();
	}
}

Sphynx::GameObject* Sphynx::Mono::GameObjectWrapper::GetFromObject_unchecked(MonoObject* obj)
{
	void* gameObject = nullptr;
	mono_field_get_value(obj, NativePtr, &gameObject);
	return (GameObject*)gameObject;
}
