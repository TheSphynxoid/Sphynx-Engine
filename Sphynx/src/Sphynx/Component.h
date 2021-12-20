#pragma once
#ifndef Sphynx_Component
#define Sphynx_Component
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
		size_t InstanceID = (size_t)this;
	public:
		virtual void Start() {};
		virtual void Update() {};
		virtual ~Component() = default;
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