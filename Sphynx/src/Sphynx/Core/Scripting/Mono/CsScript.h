#pragma once
#include "Core/Scripting/Script.h"
#include "Internal/GameObjectWrapper.h"
#include "Internal/NativeComponent.h"
#include <mono/jit/jit.h>

namespace Sphynx::Mono {
	class MonoRuntime;
	class CsScript final : public Sphynx::Core::Scripting::Script
	{
	private:
		static inline MonoClass* ComponentClass;
		static inline MonoMethod* AwakeVirtMethod, * StartVirtMethod, * UpdateVirtMethod, * FixedUpdateVirtMethod, * OnDestroyVirtMethod;
		//static inline MonoClassField* NativeField;
		static inline MonoClassField* ID;
		//The Object in mono.
		MonoObject* ScriptObject;
		MonoClass* ScriptClass;
		GameObjectWrapper* GOWrapper;
		//NativeComponent NativeComp;
		//Class name
		std::string Name;
		//Typedef of the function thunk.
		typedef void(__stdcall *UnmanagedThunk)(MonoObject*, MonoException**);
		//Functions
		UnmanagedThunk AwakeThunk;
		UnmanagedThunk StartThunk;
		UnmanagedThunk UpdateThunk;
		UnmanagedThunk FixedUpdateThunk;
		UnmanagedThunk OnDestroyThunk;

		static void Init();
		//static void HandleException(MonoException* ex);
		void SetInternalID(size_t id);
	public:
		CsScript() = default;
		CsScript(MonoObject* obj,MonoClass* objClass,std::string name);
		virtual void Awake();
		virtual void Start();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void OnDestroy();
		virtual Script* Copy();
		inline virtual const char* GetName() const noexcept {
			return Name.c_str();
		};
		static MonoClass* GetNative() { return ComponentClass; };
		friend class MonoRuntime;
		friend class GameObjectWrapper;
	};
}

