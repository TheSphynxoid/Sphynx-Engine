#pragma once
#include "Core/Scripting/Script.h"
#include <mono/jit/jit.h>

namespace Sphynx::Mono {
	class CsScript : public Sphynx::Core::Scripting::Script
	{
	private:
		MonoObject* ScriptObject;
		std::string Name;
		typedef void(*StartFunc)();
		StartFunc StartThunk;
	public:
		CsScript(MonoObject* obj,std::string name);
		virtual void Start();
		inline virtual const char* GetName() const noexcept {
			return Name.c_str();
		};
	};
}

