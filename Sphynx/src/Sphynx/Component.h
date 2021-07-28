#pragma once
#include "Object.h"
namespace Sphynx {
	class GameObject;
	class Component : public Object
	{
	private:
		virtual Component* CreateSelf() = 0;
		//Called When Component is Created and before being attached to the GameObject
		virtual void OnComponentCreate(GameObject* Parent) = 0;
		virtual void OnComponentAttach(GameObject* Parent) = 0;
		virtual void OnComponentDetach() = 0;
	public:
		virtual ~Component() = 0;
		virtual void Update() = 0;
	};
}

