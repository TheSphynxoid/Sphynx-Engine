#include "pch.h"
#include "ComponentRegistry.h"
#include "GameObject.h"
#include "Component.h"

void Sphynx::Core::Internal::ComponentRegistry::RegisterComp(GameObject* go, Component* comp)noexcept
{
	CompInfo k = { std::type_index(typeid(*comp)),comp };
	InstanceRegistry[go->GetID()].insert(k);
}

void Sphynx::Core::Internal::ComponentRegistry::RemoveComp(GameObject* go, Component* comp)noexcept
{
	CompInfo k = { std::type_index(typeid(*comp)),comp };
	InstanceRegistry[go->GetID()].erase(k);
}

bool Sphynx::Core::Internal::ComponentRegistry::IsCompInGO(GameObject* go, std::type_index typeindex)noexcept
{
	auto& r = InstanceRegistry[go->GetID()];
	return r.count({ typeindex, nullptr });
}

bool Sphynx::Core::Internal::ComponentRegistry::IsCompInGO(Sphynx::GameObject* go, Sphynx::Component* comp)noexcept
{
	return InstanceRegistry[go->GetID()].count({ std::type_index(typeid(*comp)),comp });
}

void Sphynx::Core::Internal::ComponentRegistry::CopyGameObject(GameObject* ori, GameObject* newGO)noexcept
{
	InstanceRegistry[newGO->GetID()] = InstanceRegistry[ori->GetID()];
}

void Sphynx::Core::Internal::ComponentRegistry::MoveCompsToGameObject(const Sphynx::GameObject* Src, Sphynx::GameObject* Dest)noexcept
{
	InstanceRegistry[Dest->GetID()] = InstanceRegistry[Src->GetID()];
	InstanceRegistry.erase(Src->GetID());
}

bool Sphynx::Core::Internal::ComponentRegistry::CompInfo::operator==(const CompInfo& rhs) const noexcept
{
	if (comp_ptr == nullptr || rhs.comp_ptr == nullptr) {
		return typeIndex == rhs.typeIndex;
	}
	return comp_ptr->GetID() == rhs.comp_ptr->GetID();
}
