#pragma once
#include "Object.h"
#include "Component.h"
#include "Core/Factories/ComponentFactory.h"
#include "Pointer.h"

namespace Sphynx {
	class GameObject :
		public Object
	{
	private:
		std::list<Component*> Components;

		bool IsAlive;
	public:
		//No Doubles

		template<class component>
		void AddComponent() {
			//C++17
			if (std::is_base_of_v<Component, component>) {
				if (Core::Internal::ComponentFactory::Helper::IsComponentInGameObject<component>(this)) {
					//No Parameters allowed. They will be provided to the factory.
					Components.push_back(Core::Internal::ComponentFactory::CreateComponent<component>(this));
				}
			}
		}
		template<class component>
		component* GetComponent() {
			for (auto comp : Components) {
				auto reqtid = typeid(component);
				auto comptid = typeid(*comp);
				if (reqtid == comptid) {
					return comp;
				}
			}
			return NULL;
		}
	};
}
