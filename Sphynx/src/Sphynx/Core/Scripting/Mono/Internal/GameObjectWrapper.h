#pragma once
#include "GameObject.h"
#include "TransformWrapper.h"

namespace Sphynx::Mono {
	class CsScript;
	class GameObjectWrapper final
	{
	private:
		static inline MonoClass* GameObjectClass;
		GameObject* gameObject;
		MonoObject* Managedobj;
		TransformWrapper transformWrapper;
		static inline MonoProperty* IDProp;
		static inline MonoProperty* NameProp;
		static inline MonoProperty* TransformProp;
		static inline MonoClassField* NativePtr;
		typedef void( __stdcall *AddCompThunk)(MonoObject*, MonoObject*, MonoReflectionType*, MonoException**);
		static inline AddCompThunk AddComp;
		//typedef void(__stdcall *Constructor)()
		std::vector<CsScript*> Scripts = std::vector<CsScript*>();

		static inline MonoMethod* AwakeMethod, * StartMethod, * UpdateMethod, * FixedUpdateMethod, * OnDestroyMethod;
		//Typedef of the function thunk.
		typedef void(__stdcall* UnmanagedThunk)(MonoObject*, MonoException**);
		//Functions
		UnmanagedThunk AwakeThunk;
		UnmanagedThunk StartThunk;
		UnmanagedThunk UpdateThunk;
		UnmanagedThunk FixedUpdateThunk;
		UnmanagedThunk OnDestroyThunk;
		static void Init();
	public:
		//Create from native
		GameObjectWrapper(GameObject* go);
		//Get From Managed
		GameObjectWrapper(MonoObject* obj);
		/// <summary>
		/// Adds a managed component to the gameObject.
		/// </summary>
		void AddComponent(CsScript* script);
		GameObject* GetGameObject() { return gameObject; };
		static GameObjectWrapper CreateGameObject() {};
		void Start();
		void Update();
		void FixedUpdate();
		/// <summary>
		/// Converts the MonoObject to a GameObject* without checking the type of MonoObject is "Sphynx.GameObject".
		/// </summary>
		/// <returns>A GameObject Contained by the MonoObject. Throws if Not "Sphynx.GameObject".</returns>
		static GameObject* GetFromObject_unchecked(MonoObject* obj);
		static MonoClass* GetNative() { return GameObjectClass; }
		friend class MonoRuntime;
	};
}
