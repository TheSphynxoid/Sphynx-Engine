#pragma once
#ifndef Sphynx_Component
#define Sphynx_Component
#define CompImpl(T) \
virtual const char* GetName()const noexcept override { return #T ;}

#include "Object.h"
namespace Sphynx {
	namespace Core::Internal {
		class ComponentFactory;
	}
	class GameObject;
	class Transform;
	//Native Components
	class Component : public Object
	{
	private:
		//Called When Component is Created and attached to the GameObject.
		virtual void OnComponentAttach(GameObject* parent) = 0;
		virtual void OnComponentDetach() {};
		GameObject* Parent;
		Transform* _transform;
		EntityID InstanceID = (EntityID)0;
	public:
		virtual void Start() {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual ~Component() = default;
		virtual const char* GetName()const noexcept = 0;
		virtual Component* CopyInstance() = 0;
		GameObject* GetGameObject() { return Parent; };
		size_t GetID() { return InstanceID; };
		Transform* GetTransform() {
			return _transform; 
		};
		bool operator==(Component* comp) {
			return InstanceID == comp->InstanceID;
		}
		bool operator!=(Component* comp) {
			return (this->InstanceID != comp->InstanceID);
		}
		friend Core::Internal::ComponentFactory;
		friend GameObject;
	};
}
#else
#endif