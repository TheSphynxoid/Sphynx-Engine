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
			auto comp = new component(SPH_FORWARD(args)...);
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
			constexpr static bool CompareTypeToComponant_typeindex(Component* c)noexcept {
				return (std::type_index(typeid(comp) == std::type_index(typeid(*c))));
			}
			template<class comp>
			constexpr static bool CompareTypeToComponant_typeid(Component* c)noexcept {
				return (typeid(comp) == typeid(*c));
			}
			template<class comp>
			static bool IsComponentInGameObject(Sphynx::GameObject* obj)noexcept {
				return ComponentRegistry::IsCompInGO(obj, std::type_index(typeid(comp)));
			}
			static bool IsComponentInGameObject(Sphynx::GameObject* obj, Component* comp)noexcept {
				return ComponentRegistry::IsCompInGO(obj, comp);
			}
			template<class comp>
			constexpr static bool IsComponentOfType(Component* component)noexcept {
				return typeid(comp) == typeid(*component);
			}
		};
	};

	//Changed an answer in GameDev.StackExchange
	typedef std::unordered_map<std::string, CreateComponentFunc> Registry;

	template<class T>
	Component* createComponent() {
		return new T;
	}

	template<class T>
	struct RegistryEntry
	{
	public:

		constexpr static RegistryEntry<T>& Instance(const std::string& name) noexcept
		{
			// Because I use a singleton here, even though `COMPONENT_REGISTER`
			// is expanded in multiple translation units, the constructor
			// will only be executed once. Only this cheap `Instance` function
			// (which most likely gets inlined) is executed multiple times.
			static RegistryEntry<T> inst(name);
			return inst;
		}

	private:
		RegistryEntry(const std::string& name)
		{
			Registry& reg = ComponentRegistry::TypeRegistry;
			CreateComponentFunc func = createComponent<T>;

			std::pair<Registry::iterator, bool> ret =
				reg.insert(Registry::value_type(name, func));

			if (ret.second == false) {
				//Registration errors cannot be allowed.
				static_assert("Component Registry Error.");
			}
		}

		RegistryEntry(const RegistryEntry<T>&) = delete; // C++11 feature
		RegistryEntry& operator=(const RegistryEntry<T>&) = delete;
	};
}
#else
#endif