#include "pch.h"
#include "ScriptComponent.h"
#include "Mono/MonoRuntime.h"
#include "Mono/Internal/GameObjectWrapper.h"
#include "Mono/Internal/ScriptManager.h"

Sphynx::Core::ScriptComponent::ScriptComponent()
{

}

void Sphynx::Core::ScriptComponent::Start()
{
	GOWrapper->Start();
}

void Sphynx::Core::ScriptComponent::Update()
{
	GOWrapper->Update();
}

void Sphynx::Core::ScriptComponent::FixedUpdate()
{
	GOWrapper->FixedUpdate();
}

void Sphynx::Core::ScriptComponent::OnComponentAttach(GameObject* parent)
{
	GOWrapper = Mono::ScriptManager::GetGameobjectWrapper(GetGameObject());
	//GOWrapper->AddComponent((Mono::CsScript*)script);
	for (auto s : scripts) {
		GOWrapper->AddComponent((Mono::CsScript*)s);
	}
	//GOWrapper->Awake();
}

void Sphynx::Core::ScriptComponent::OnComponentDetach()
{
	//script->OnDestroy();
}

void Sphynx::Core::ScriptComponent::AddComponent(std::string name)
{
	//TODO: Change later make it backend independent
	//GOWrapper = Mono::GameObjectWrapper(GetGameObject());

	script = Mono::MonoRuntime::CreateScriptByName(name);
}

const char* Sphynx::Core::ScriptComponent::GetName()
{
	return script->GetName();
}
