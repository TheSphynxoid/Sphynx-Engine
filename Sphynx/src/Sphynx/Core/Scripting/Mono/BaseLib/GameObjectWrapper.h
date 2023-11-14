#pragma once
#include "GameObject.h"

class MonoObject;

namespace Sphynx::Mono {
	class GameObjectWrapper
	{
	private:
		GameObject* gameObject;
		MonoObject* Managedobj;
		//TransformWrapper* transform;
		static inline MonoProperty* IDProp;
		static inline MonoProperty* NameProp;
		//static inline MonoProperty TransformProp;
	public:
		GameObjectWrapper(GameObject* go);

		static GameObjectWrapper CreateGameObject() {};
	};
}
