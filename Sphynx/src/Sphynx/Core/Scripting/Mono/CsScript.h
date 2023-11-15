#pragma once
#include "Core/Scripting/Script.h"
#include "Internal/GameObjectWrapper.h"
#include <mono/jit/jit.h>

namespace Sphynx::Mono {
	class MonoRuntime;
	class CsScript final : public Sphynx::Core::Scripting::Script
	{
	private:
		//A pointer the runtime.
		static inline MonoRuntime* Runtime;
		static inline MonoMethod* AwakeVirtMethod, * StartVirtMethod, * UpdateVirtMethod, * FixedUpdateVirtMethod, * OnDestroyVirtMethod;
		//The Object in mono.
		MonoObject* ScriptObject;
		GameObjectWrapper* GOWrapper;
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

		static void HandleException(MonoException* ex);
	public:
		CsScript() = default;
		CsScript(MonoObject* obj,std::string name);
		virtual void Awake();
		virtual void Start();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void OnDestroy();
		virtual Script* Copy();
		inline virtual const char* GetName() const noexcept {
			return Name.c_str();
		};
		inline virtual Core::Scripting::ScriptingBackend* GetScriptingBackend() { return (Core::Scripting::ScriptingBackend*)Runtime; };
		friend MonoRuntime;
	};
}

