#include "pch.h"
#include "GameObjectWrapper.h"
#include "../MonoRuntime.h"

Sphynx::Mono::GameObjectWrapper::GameObjectWrapper(GameObject* go)
{
	gameObject = go;
	auto GoClass = MonoRuntime::GetCommonType("Sphynx.GameObject");
	Managedobj = MonoRuntime::CreateObject(GoClass);
	for (auto& comp : go->GetComponents()) {
		if (comp->GetName() == "MeshRenderer") {

		}
	}
}

void Sphynx::Mono::GameObjectWrapper::AddComponent(CsScript* script)
{
	Scripts.push_back(script);
	MonoException* ex;
	AddComp(this->Managedobj, script->ScriptObject, script->ScriptClass, &ex);
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
