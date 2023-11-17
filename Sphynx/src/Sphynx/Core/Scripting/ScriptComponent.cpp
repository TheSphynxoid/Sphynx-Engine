#include "pch.h"
#include "ScriptComponent.h"
#include "Mono/MonoRuntime.h"
#include "Mono/Internal/GameObjectWrapper.h"
#include "Mono/Internal/ScriptManager.h"

Sphynx::Core::ScriptComponent::ScriptComponent(std::string name)
{
	//TODO: Change later make it backend independent
	//GOWrapper = Mono::GameObjectWrapper(GetGameObject());

	script = Mono::MonoRuntime::CreateScriptByName(name);
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
	GOWrapper = Mono::ScriptManager::GetGameobjectWrapper(GetGameObject());
	GOWrapper->AddComponent((Mono::CsScript*)script);
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
