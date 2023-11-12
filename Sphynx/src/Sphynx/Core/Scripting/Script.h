#pragma once
#include "Component.h"


namespace Sphynx::Core::Scripting {
	class ScriptingBackend;
	class Script
	{
	public:
		virtual void Start() = 0;
		virtual Script* Copy() = 0;
		virtual const char* GetName() const noexcept = 0;
		virtual ScriptingBackend* GetScriptingBackend() = 0;
	};
}