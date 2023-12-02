#pragma once
#include "GameObject.h"
#include "TransformWrapper.h"

namespace Sphynx::Mono {
	class CsScript;
	class GameObjectWrapper
	{
	private:
		static inline MonoClass* GameObjectClass;
		GameObject* gameObject;
		MonoObject* Managedobj;
		TransformWrapper transform;
		static inline MonoProperty* IDProp;
		static inline MonoProperty* NameProp;
		static inline MonoProperty* TransformProp;
		static inline MonoClassField* NativePtr;
		typedef void( __stdcall *AddCompThunk)(MonoObject*, MonoObject*, MonoReflectionType*, MonoException**);
		static inline AddCompThunk AddComp;
		std::vector<CsScript*> Scripts = std::vector<CsScript*>();
		static void Init();
	public:
		//Create from native
		GameObjectWrapper(GameObject* go);
		//Get From Managed
		GameObjectWrapper(MonoObject* obj);
		void AddComponent(CsScript* script);
		GameObject* GetGameObject() { return gameObject; };
		static GameObjectWrapper CreateGameObject() {};
		void Start();
		void Update();
		void FixedUpdate();
		static GameObject* GetFromObject_unchecked(MonoObject* obj);
		static MonoClass* GetNative() { return GameObjectClass; }
		friend class MonoRuntime;
	};
}
