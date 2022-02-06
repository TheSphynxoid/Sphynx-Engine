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
		std::string Name = "GameObject";
	protected:
		Transform* transform = new Transform();
	public:
		static const GameObject PlaceHolder;
		GameObject(const char* name = "GameObject");
		GameObject(const GameObject& obj)noexcept;
		GameObject(GameObject&& obj)noexcept;
		~GameObject();
		GameObject& operator=(GameObject&& obj)noexcept;
		GameObject& operator=(const GameObject& obj)noexcept;
		//TODO: Callback to Scene to handle object delete.
		void Destroy() { IsAlive = false; };
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
						Core::Internal::ComponentFactory::RemoveComponent(this, comp);
						Components.remove(comp);
						return;
					}
				}
			}
		}
		static GameObject CreatePrimitive(Primitives primitive , const char* name = "GameObject");
		Transform* GetTransform() { 
			return transform;
		};
		const char* GetName() { return Name.c_str(); };
		void SetName(const std::string& n) { Name = n; };
		std::list<Component*> GetComponents() { return Components; };
		size_t GetID() { return InstanceID; };
		bool operator==(const GameObject& other) {
			if (IsAlive == false && other.IsAlive == IsAlive)return true;
			return InstanceID == other.InstanceID && Components == other.Components;
		}
		bool operator!=(const GameObject& other) {
			return !(*this == other);
		}
	};
}
