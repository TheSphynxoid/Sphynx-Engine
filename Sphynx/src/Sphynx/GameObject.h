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
		void Update() {
			for (auto comp : Components) {
				comp->Update();
			}
		}
		bool IsAlive;
	public:
		//No Double
		template<class component>
		void AddComponent() {
			//C++17
			if (std::is_base_of_v<Component, component>) {
				if (Core::Internal::ComponentFactory::ComponentHelper::IsComponentInGameObject<component>(this)) {
					//No Parameters allowed. They will be provided to the factory.
					//Components Should not have consturctors and they will be ignored by the factory.
					Components.push_back(Core::Internal::ComponentFactory::CreateComponent<component>(this));
				}
			}
		}
		template<class component>
		component* GetComponent() {
			if (!std::is_base_of_v<Component, component>) return NULL;
			for (auto comp : Components) {
				if (Core::Internal::ComponentFactory::ComponentHelper::CompareTypeToComponant_typeid<component>(comp)) {
					return static_cast<component*>(comp);
				}
			}
			return NULL;
		}
	};
}
