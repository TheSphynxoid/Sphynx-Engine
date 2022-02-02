#ifndef Sphynx_ComponentFactory
#define Sphynx_ComponentFactory
#include "Component.h"
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
			//We Don't Check Because GameObject Does That.
			auto comp = new component(std::forward<Args>(args)...);
			comp->Parent = object;
			comp->_transform = object->GetTransform();
			((Component*)comp)->OnComponentAttach(object);
			//TypeIndex List
			ComponentRegistry::RegisterComp(object, comp);
			return comp;
		}
		static void RemoveComponent(Sphynx::GameObject* go, Component* comp) {
			ComponentRegistry::RemoveComp(go, comp);
			comp->OnComponentDetach();
			delete comp;
		}
		static void CompCopy(GameObject* ori, GameObject* cpy) {
			ComponentRegistry::CopyGameObject(ori, cpy);
		}

		//Utility
		typedef struct ComponentHelper {
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
		}ComponentHelper;
	};
}
#else
#endif