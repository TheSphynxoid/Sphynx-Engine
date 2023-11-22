#include "pch.h"
#include "GameObjectWrapper.h"
#include "../MonoRuntime.h"

Sphynx::Mono::GameObjectWrapper::GameObjectWrapper(GameObject* go)
{
	gameObject = go;
	Managedobj = MonoRuntime::CreateObject(GameObjectClass);
	size_t ID = gameObject->GetID();
	mono_property_set_value(IDProp, Managedobj, (void**)&ID, nullptr);
	for (auto& comp : go->GetComponents()) {
		if (comp->GetName() == "Transform") {
			transform = TransformWrapper();
		}
	}
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
