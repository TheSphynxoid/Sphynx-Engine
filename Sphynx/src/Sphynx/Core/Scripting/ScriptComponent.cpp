#include "pch.h"
#include "ScriptComponent.h"
#include "Mono/MonoRuntime.h"

Sphynx::Core::ScriptComponent::ScriptComponent(std::string name)
{
	//TODO: Change later make it backend independent
	script = Mono::MonoRuntime::CreateScriptByName(name);
}

void Sphynx::Core::ScriptComponent::OnComponentAttach(GameObject* parent)
{
	script->Start();
}

const char* Sphynx::Core::ScriptComponent::GetName()
{
	return script->GetName();
}
