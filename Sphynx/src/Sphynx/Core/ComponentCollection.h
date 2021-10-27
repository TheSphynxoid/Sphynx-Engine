#pragma once
#include "Component.h"
#include "Factories/ComponentFactory.h"
#include <set>

namespace Sphynx::Core::Internal {
	//Ignore.
	class ComponentCollection
	{
	private:
		std::list<Component*> Comps;
		std::set<std::type_index> TypeIndex;
	public:
		ComponentCollection() = default;
		void AddComponent(Component* comp, std::type_index index) {
			Comps.push_back(comp);
			TypeIndex.insert(index);
		}
		void RemoveComponent(std::type_index index) {
		}
		Component* FindComponent(std::type_index index) {

		}

	};
}