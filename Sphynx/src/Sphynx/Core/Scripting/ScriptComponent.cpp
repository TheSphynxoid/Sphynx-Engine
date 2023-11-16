#include "pch.h"
#include "ScriptComponent.h"
#include "Mono/MonoRuntime.h"
#include "Mono/Internal/GameObjectWrapper.h"

Sphynx::Core::ScriptComponent::ScriptComponent(std::string name)
{
	//TODO: Change later make it backend independent
	//GOWrapper = Mono::GameObjectWrapper(GetGameObject());

	script = Mono::MonoRuntime::CreateScriptByName(name);
	GOWrapper->AddComponent((Mono::CsScript*)script);
}

void Sphynx::Core::ScriptComponent::Start()
{
	script->Start();
}

void Sphynx::Core::ScriptComponent::Update()
{
	script->Update();
}

void Sphynx::Core::ScriptComponent::FixedUpdate()
{
	script->FixedUpdate();
}

void Sphynx::Core::ScriptComponent::OnComponentAttach(GameObject* parent)
{
	script->Awake();
}

void Sphynx::Core::ScriptComponent::OnComponentDetach()
{
	script->OnDestroy();
}

const char* Sphynx::Core::ScriptComponent::GetName()
{
	return script->GetName();
}
