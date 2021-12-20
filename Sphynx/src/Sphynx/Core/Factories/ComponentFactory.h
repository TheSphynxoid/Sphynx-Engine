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
		inline static std::unordered_map<GameObject*,std::unordered_set<std::type_index>> RegisteredTypes;
	public:
		template<typename component, typename ...Args>
		static component* CreateComponent(GameObject* object, Args&& ...args) {
			//We Don't Check Because GameObject Does That.
			auto comp = new component(std::forward<Args>(args)...);
			comp->Parent = object;
			comp->_transform = object->GetTransform();
			((Component*)comp)->OnComponentAttach(object);
			//TypeIndex List
			auto til = RegisteredTypes[object];
			til.insert(std::type_index(typeid(component)));
			return comp;
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
			static bool IsComponentInGameObject(GameObject* obj) {
				return RegisteredTypes[obj].find(std::type_index(typeid(comp))) != RegisteredTypes[obj].end();
			}
		}ComponentHelper;
	};
}
#else
#endif