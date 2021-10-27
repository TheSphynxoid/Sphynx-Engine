#pragma once
#ifndef Sphynx_Component
#define Sphynx_Component
#include "Object.h"
namespace Sphynx {
	class GameObject;
	class Transform;
	//Native Components
	class Component : public Object
	{
	private:
		//Called When Component is Created and attached to the GameObject.
		virtual void OnComponentAttach(GameObject* parent) = 0;
		virtual void OnComponentDetach() = 0;
		GameObject* Parent;
		Transform* _transform;
		size_t InstanceID = (size_t)this;
	public:
		virtual ~Component() = 0;
		virtual void Update() = 0;
		GameObject* GetGameObject() { return Parent; };
		Transform* GetTransform() {
			return _transform; 
		};
		friend Core::Internal::ComponentFactory;
		friend GameObject;
	};
}
#else
#endif