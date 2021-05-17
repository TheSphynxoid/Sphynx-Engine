#pragma once
#include "Component.h"
#include "GameObject.h"
#include "../TestComponent.h"
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
namespace Sphynx::Core::Internal {
	class ComponentFactory
	{
		static std::unordered_map<GameObject*,std::unordered_set<std::type_index>> RegisteredTypeIDs;
	public:
		template<typename component>
		static Component* CreateComponent(GameObject* object) {
			//We Don't Check Because GameObject Does That.
			auto comp = component::CreateSelf();
			comp.OnComponentCreate(object);
			//Type Index List
			auto til = RegisteredTypeIDs[object];
			//if (!til.empty) {
			//	til.insert(std::type_index(typeid(component)));
			//}
			til.insert(std::type_index(typeid(component)));
		}
		//Utility
		struct Helper {
			template<class comp>
			static bool CompareTypeToComponant_typeindex(Component* c) {
				return (std::type_index(typeid(comp) == std::type_index(typeid(*c));
			}
			template<class comp>
			static bool CompareTypeToComponant_typeid(Component* c) {
				return (typeid(comp) == typeid(*c));
			}
			template<class comp>
			static bool IsComponentInGameObject(GameObject* obj) {
				return RegisteredTypeIDs[obj].find(std::type_index(typeid(comp))) != RegisteredTypeIDs.end();
			}
		};
		friend class Component;
	};
}