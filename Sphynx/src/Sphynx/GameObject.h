#pragma once
#include "Object.h"
#include "Component.h"
#include "Core/Factories/ComponentFactory.h"
#include "Pointer.h"
#include "Core/Transform.h"

namespace Sphynx {
	class Transform;
	enum class Primitives {
		Sphere, Capsule, Cube, Plane, Triangle
	};
	//Native GameObject
	class GameObject final :
		public Object
	{
		std::list<Component*> Components = std::list<Component*>();
		EntityID InstanceID = (EntityID)0;
		std::string Name = "GameObject";
		Transform* transform = new Transform();
		bool IsAlive = false;
	public:
		static const GameObject PlaceHolder;
		GameObject(const char* name = "GameObject");
		GameObject(const GameObject& obj)noexcept;
		GameObject(GameObject&& obj)noexcept;
		~GameObject();
		GameObject& operator=(GameObject&& obj)noexcept;
		GameObject& operator=(const GameObject& obj)noexcept;
		void Destroy();
		bool IsActive() { return IsAlive; };
		void Start() {
			if (IsAlive) {
				for (auto comp : Components) {
					comp->Start();
				}
			}
		}
		void Update() {
			if (IsAlive) {
				for (auto comp : Components) {
					comp->Update();
				}
			}
		}
		//No Double
		template<class component, class ...Args>
		std::enable_if_t<std::is_base_of_v<Sphynx::Component, component>, component*> AddComponent(Args&& ...args) {
			if (!Core::Internal::ComponentFactory::Helper::IsComponentInGameObject<component>(this)) {
				auto rtval = Core::Internal::ComponentFactory::CreateComponent<component>(this, SPH_FORWARD(args)...);
				Components.push_back(rtval);
				return rtval;
			}
			return nullptr;
		}
		template<class component>
		std::enable_if_t<std::is_base_of_v<Sphynx::Component, component>,component*> GetComponent() {
			for (auto comp : Components) {
				if (Core::Internal::ComponentFactory::Helper::CompareTypeToComponant_typeid<component>(comp)) {
					return static_cast<component*>(comp);
				}
			}
			return nullptr;
		}
		template<class component>
		std::enable_if_t<std::is_base_of_v<Sphynx::Component, component>, void> RemoveComponent() {
			for (auto comp : Components) {
				if (Core::Internal::ComponentFactory::Helper::CompareTypeToComponant_typeid<component>(comp)) {
					Core::Internal::ComponentFactory::RemoveComponent(this, comp);
					Components.remove(comp);
					return;
				}
			}
		}
		static GameObject* CreatePrimitive(Primitives primitive , std::string name = "GameObject");
		Transform* GetTransform() { 
			return transform;
		};
		const char* GetName() { return Name.c_str(); };
		void SetName(const std::string& n) { Name = n; };
		std::list<Component*> GetComponents() { return Components; };
		size_t GetID() const noexcept{ return InstanceID; };
		bool operator==(const GameObject& other) {
			if (IsAlive == false && other.IsAlive == IsAlive)return true;
			return InstanceID == other.InstanceID && Components == other.Components;
		}
		bool operator!=(const GameObject& other) {
			return !(*this == other);
		}
	};
}
