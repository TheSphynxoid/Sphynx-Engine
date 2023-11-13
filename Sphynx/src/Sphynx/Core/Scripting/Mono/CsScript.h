#pragma once
#include "Core/Scripting/Script.h"
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
		//Class name
		std::string Name;
		//Typedef of the Start function thunk.
		typedef void(__stdcall *StartFunc)(MonoObject*, MonoException**);
		//Start function thunk.
		StartFunc StartThunk;
		//Typedef of the Update function thunk.
		typedef void(__stdcall *UpdateFunc)(MonoObject*, MonoException**);
		//Update function thunk.
		StartFunc UpdateThunk;
		//This is to validate the existance of ScriptObject.
		bool IsValid = false;

		static void HandleException(MonoException* ex);
	public:
		CsScript() = default;
		CsScript(MonoObject* obj,std::string name);
		virtual void Start();
		virtual Script* Copy();
		inline virtual const char* GetName() const noexcept {
			return Name.c_str();
		};
		inline virtual Core::Scripting::ScriptingBackend* GetScriptingBackend() { return (Core::Scripting::ScriptingBackend*)Runtime; };
		friend MonoRuntime;
	};
}

