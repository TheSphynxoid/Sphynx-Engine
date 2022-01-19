#ifndef Sphynx_ComponentFactory
#define Sphynx_ComponentFactory
#include "Component.h"
#include "GameObject.h"
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
namespace Sphynx::Core::Internal {
	class ComponentFactory final
	{
		inline static std::unordered_map<GameObject*, std::unordered_set<size_t>> RegisteredTypes;
		inline static std::unordered_map<GameObject*, std::unordered_set<std::type_index>> RegisteredTypesIndex;
	public:
		template<typename component, typename ...Args>
		static component* CreateComponent(Sphynx::GameObject* object, Args&& ...args) {
			//We Don't Check Because GameObject Does That.
			auto comp = new component(std::forward<Args>(args)...);
			comp->Parent = object;
			comp->_transform = object->GetTransform();
			((Component*)comp)->OnComponentAttach(object);
			//TypeIndex List
			auto til = RegisteredTypes[object];
			til.insert(comp->GetID());
			auto goIdx = RegisteredTypesIndex[object];
			goIdx.insert(std::type_index(typeid(component)));
			return comp;
		}

		static void CompMove(GameObject* old, GameObject* newobj) {
			auto a = RegisteredTypes[old];
			RegisteredTypes[newobj] = RegisteredTypes[old];
			RegisteredTypes.erase(old);
			RegisteredTypesIndex[newobj] = RegisteredTypesIndex[old];
			RegisteredTypesIndex.erase(old);
		}

		static void CompCopy(GameObject* ori, GameObject* cpy) {
			RegisteredTypes[cpy] = RegisteredTypes[ori];
			RegisteredTypesIndex[cpy] = RegisteredTypesIndex[ori];
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
				return RegisteredTypesIndex[obj].find(std::type_index(typeid(comp))) != RegisteredTypesIndex[obj].end();
			}
			static bool IsComponentInGameObject(Sphynx::GameObject* obj, Component* comp) {
				return RegisteredTypes[obj].find(comp->GetID()) != RegisteredTypes[obj].end();
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