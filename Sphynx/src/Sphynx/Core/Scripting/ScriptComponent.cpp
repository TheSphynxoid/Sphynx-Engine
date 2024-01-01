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
	for (auto s : scripts) {
		GOWrapper->AddComponent((Mono::CsScript*)s);
	}
}

void Sphynx::Core::ScriptComponent::OnComponentDetach()
{
	//script->OnDestroy();
}

void Sphynx::Core::ScriptComponent::AddScript(std::string name)
{
	//TODO: Change later make it backend independent
	auto s = Mono::MonoRuntime::CreateScriptByName(name);
	GOWrapper->AddComponent((Mono::CsScript*)s);
	scripts.push_back(s);
}
