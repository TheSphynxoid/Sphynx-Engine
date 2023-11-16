#pragma once
#include "GameObject.h"


struct _MonoObject;
typedef _MonoObject MonoObject;
struct _MonoProperty;
typedef _MonoProperty MonoProperty;

namespace Sphynx::Mono {
	class CsScript;
	class GameObjectWrapper
	{
	private:
		GameObject* gameObject;
		MonoObject* Managedobj;
		//TransformWrapper* transform;
		static inline MonoProperty* IDProp;
		static inline MonoProperty* NameProp;
		//static inline MonoProperty TransformProp;

		typedef void(*AddCompThunk)(MonoObject*, MonoObject*, MonoClass*, MonoException**);
		static inline AddCompThunk AddComp;
		std::vector<CsScript*> Scripts;
	public:
		GameObjectWrapper(GameObject* go);
		void AddComponent(CsScript* script);
		GameObject* GetGameObject() { return gameObject; };
		static GameObjectWrapper CreateGameObject() {};
		void Start();
		void Update();
		void FixedUpdate();

		friend class MonoRuntime;
	};
}
