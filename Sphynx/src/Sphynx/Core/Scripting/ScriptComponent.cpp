#include "pch.h"
#include "ScriptComponent.h"

Sphynx::Core::ScriptComponent::ScriptComponent(std::string path)
{

}

void Sphynx::Core::ScriptComponent::OnComponentAttach(GameObject* parent)
{
	script->Start();
}

const char* Sphynx::Core::ScriptComponent::GetName()
{
	return script->GetName();
}
