#include "pch.h"
#include "ScriptComponent.h"
#include "Mono/MonoRuntime.h"
#include "Mono/Internal/GameObjectWrapper.h"
#include "Mono/Internal/ScriptManager.h"

Sphynx::Core::ScriptComponent::ScriptComponent(std::string scriptName)
{
	script = Mono::MonoRuntime::CreateScriptByName(scriptName, GetID());
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
	GOWrapper = Mono::ScriptManager::GetGameobjectWrapper(this->GetGameObject());
	GOWrapper->AddComponent((Mono::CsScript*)script);
	script->Awake();
}

void Sphynx::Core::ScriptComponent::OnComponentDetach()
{
	script->OnDestroy();
}
