#pragma once
#include "GameObject.h"


struct _MonoObject;
typedef _MonoObject MonoObject;
struct _MonoProperty;
typedef _MonoProperty MonoProperty;
struct _MonoClass;
typedef _MonoClass MonoClass;
struct _MonoException;
typedef _MonoException MonoException;

namespace Sphynx::Mono {
	class CsScript;
	class GameObjectWrapper
	{
	private:
		static inline MonoClass* GameObjectClass;
		GameObject* gameObject;
		MonoObject* Managedobj;
		//TransformWrapper* transform;
		static inline MonoProperty* IDProp;
		static inline MonoProperty* NameProp;
		//static inline MonoProperty TransformProp;

		typedef void( __stdcall *AddCompThunk)(MonoObject*, MonoObject*, MonoClass*, MonoException**);
		static inline AddCompThunk AddComp;
		std::vector<CsScript*> Scripts = std::vector<CsScript*>();
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
