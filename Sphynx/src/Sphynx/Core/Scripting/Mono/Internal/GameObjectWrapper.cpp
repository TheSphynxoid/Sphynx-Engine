#include "pch.h"
#include "GameObjectWrapper.h"
#include "../MonoRuntime.h"

void Sphynx::Mono::GameObjectWrapper::Init()
{
	auto addcomp = mono_class_get_method_from_name(GameObjectClass, "InternalAddComp", 2);
	AddComp = (AddCompThunk)mono_method_get_unmanaged_thunk(addcomp);
	IDProp = mono_class_get_property_from_name(GameObjectClass, "ID");
	NameProp = mono_class_get_property_from_name(GameObjectClass, "Name");
	NativePtr = mono_class_get_field_from_name(GameObjectClass, "NativePtr");
}

Sphynx::Mono::GameObjectWrapper::GameObjectWrapper(GameObject* go)
{
	gameObject = go;
	Managedobj = MonoRuntime::CreateObject(GameObjectClass);
	size_t ID = gameObject->GetID();
	mono_property_set_value(IDProp, Managedobj, (void**)&ID, nullptr);
	auto name = mono_string_new(MonoRuntime::GetAppdomain(), gameObject->GetName());
	mono_property_set_value(NameProp, Managedobj, (void**)&name, nullptr);
	mono_field_set_value(Managedobj, NativePtr, gameObject);
	for (auto& comp : go->GetComponents()) {
		if (comp->GetName() == "Transform") {
			transform = TransformWrapper();
		}
	}
}

Sphynx::Mono::GameObjectWrapper::GameObjectWrapper(MonoObject* obj)
{
	if (mono_object_get_class(obj) != GameObjectClass) {
		Core_Error("Invalid MonoObject. Is not GameObject class");
		return;
	}
	Managedobj = obj;
	mono_field_get_value(obj, NativePtr, gameObject);
}

void Sphynx::Mono::GameObjectWrapper::AddComponent(CsScript* script)
{
	Scripts.push_back(script);
	auto ScriptReflection = mono_type_get_object(MonoRuntime::GetAppdomain(), mono_class_get_type(script->ScriptClass));
	MonoException* ex;
	AddComp(this->Managedobj, script->ScriptObject, ScriptReflection , &ex);
	if (ex != nullptr) {
		mono_print_unhandled_exception((MonoObject*)ex);
		mono_raise_exception(ex);
		//
		throw;
	}
}

void Sphynx::Mono::GameObjectWrapper::Start()
{
	for (auto script : Scripts) {
		script->Start();
	}
}

void Sphynx::Mono::GameObjectWrapper::Update()
{
	for (auto script : Scripts) {
		script->Update();
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
	GameObject* gameObject;
	mono_field_get_value(obj, NativePtr, gameObject);
	return gameObject;
}
