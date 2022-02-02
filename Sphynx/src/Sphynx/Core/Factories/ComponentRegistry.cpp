#include "pch.h"
#include "ComponentRegistry.h"
#include "GameObject.h"
#include "Component.h"

void Sphynx::Core::Internal::ComponentRegistry::RegisterComp(GameObject* go, Component* comp)
{
	CompInfo k = { std::type_index(typeid(*comp)),comp };
	Registry[go->GetID()].insert(k);
}

void Sphynx::Core::Internal::ComponentRegistry::RemoveComp(GameObject* go, Component* comp)
{
	CompInfo k = { std::type_index(typeid(*comp)),comp };
	Registry[go->GetID()].erase(k);
}

bool Sphynx::Core::Internal::ComponentRegistry::IsCompInGO(GameObject* go, std::type_index typeindex)
{
	auto& r = Registry[go->GetID()];
	return r.count({ typeindex, nullptr });
}

bool Sphynx::Core::Internal::ComponentRegistry::IsCompInGO(Sphynx::GameObject* go, Sphynx::Component* comp)
{
	return Registry[go->GetID()].count({ std::type_index(typeid(*comp)),comp });
}

void Sphynx::Core::Internal::ComponentRegistry::CopyGameObject(GameObject* ori, GameObject* newGO)
{
	Registry[newGO->GetID()] = Registry[ori->GetID()];
}

bool Sphynx::Core::Internal::ComponentRegistry::CompInfo::operator==(const CompInfo& rhs) const
{
	if (comp_ptr == nullptr || rhs.comp_ptr == nullptr) {
		return typeindex == rhs.typeindex;
	}
	return comp_ptr->GetID() == rhs.comp_ptr->GetID();
}
