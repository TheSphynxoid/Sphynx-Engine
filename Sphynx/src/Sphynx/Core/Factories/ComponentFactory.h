#pragma once
#ifndef Sphynx_ComponentFactory
#define Sphynx_ComponentFactory
#include "GameObject.h"
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include "ComponentRegistry.h"
namespace Sphynx::Core::Internal {
	class ComponentFactory final
	{
	public:
		template<typename component, typename ...Args>
		static component* CreateComponent(Sphynx::GameObject* object, Args&& ...args) {
			//We don't check if it's a valid Component type because GameObject does that.
			auto comp = new component(SPH_Forward(args)...);
			comp->Parent = object;
			comp->InstanceID = EntityRegistry::GetID(comp, EntityFlag::Component);
			comp->_transform = object->GetTransform();
			((Component*)comp)->OnComponentAttach(object);
			//TypeIndex List
			ComponentRegistry::RegisterComp(object, comp);
			return comp;
		}
		static void RemoveComponent(Sphynx::GameObject* go, Component* comp) {
			ComponentRegistry::RemoveComp(go, comp);
			comp->OnComponentDetach();
			EntityRegistry::ReturnID(comp->InstanceID);
			//Clean-up.
			delete comp;
		}
		static void CompCopy(GameObject* ori, GameObject* cpy) {
			ComponentRegistry::CopyGameObject(ori, cpy);
		}
		static void CompMove(const GameObject* Src, GameObject* movedTo) {
			ComponentRegistry::MoveCompsToGameObject(Src, movedTo);
		}
		//Utility
		struct Helper {
			template<class comp>
			static bool CompareTypeToComponant_typeindex(Component* c) {
				return (std::type_index(typeid(comp) == std::type_index(typeid(*c))));
			}
			template<class comp>
			static bool CompareTypeToComponant_typeid(Component* c) {
				return (typeid(comp) == typeid(*c));
			}
			template<class comp>
			static bool IsComponentInGameObject(Sphynx::GameObject* obj) {
				return ComponentRegistry::IsCompInGO(obj, std::type_index(typeid(comp)));
			}
			static bool IsComponentInGameObject(Sphynx::GameObject* obj, Component* comp) {
				return ComponentRegistry::IsCompInGO(obj, comp);
			}
			template<class comp>
			static bool IsComponentOfType(Component* component) {
				return typeid(comp) == typeid(*component);
			}
		};
	};
}
#else
#endif