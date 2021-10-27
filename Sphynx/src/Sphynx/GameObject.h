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
	class GameObject :
		public Object
	{
	private:
		std::list<Component*> Components = std::list<Component*>();
		size_t InstanceID = 0;
		bool IsAlive = false;
	protected:
		Transform* transform = new Transform();
	public:
		static const GameObject PlaceHolder;
		GameObject();
		GameObject(const GameObject& obj)noexcept;
		GameObject(GameObject&& obj)noexcept;
		GameObject& operator=(GameObject&& obj)noexcept;
		GameObject& operator=(const GameObject& obj)noexcept;
		bool IsActive() { return IsAlive; };
		void Update() {
			for (auto comp : Components) {
				comp->Update();
			}
		}
		//No Double
		template<class component, class ...Args>
		void AddComponent(Args&& ...args) {
			//C++17
			if (std::is_base_of_v<Component, component>) {
				if (!Core::Internal::ComponentFactory::ComponentHelper::IsComponentInGameObject<component>(this)) {
					Components.push_back(Core::Internal::ComponentFactory::CreateComponent<component>(this, std::forward<Args>(args)...));
				}
			}
		}
		template<class component>
		component* GetComponent() {
			if (std::is_base_of_v<Component, component>)
			{
				for (auto comp : Components) {
					if (Core::Internal::ComponentFactory::ComponentHelper::CompareTypeToComponant_typeid<component>(comp)) {
						return static_cast<component*>(comp);
					}
				}
			}
			return nullptr;
		}
		template<class component>
		void RemoveComponent() {
			if (!std::is_base_of_v<Component, component>) return;
			else {
				for (auto comp : Components) {
					if (Core::Internal::ComponentFactory::ComponentHelper::CompareTypeToComponant_typeid<component>(comp)) {
						Components.remove(comp);
						return;
					}
				}
			}
		}
		static GameObject CreatePrimitive(Primitives primitive);
		Transform* GetTransform() { 
			return transform;
		};
		uintptr_t GetID() { return InstanceID; };
		bool operator==(const GameObject& other) {
			if (IsAlive == false && other.IsAlive == IsAlive)return true;
			return InstanceID == other.InstanceID && Components == other.Components;
		}
		bool operator!=(const GameObject& other) {
			return !(*this == other);
		}
	};
}
