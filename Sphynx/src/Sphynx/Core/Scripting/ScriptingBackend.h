#pragma once
#include "System.h"
#include "GameObject.h"

namespace Sphynx::Core::Scripting {
	class Script;
	class ScriptingBackend : public System {
	private:
	public:
		inline UpdateOn GetSystemUpdateOn() override { return UpdateOn::OnUpdate; };

		//static Script* CreateScript(const char* path, GameObject GO);
		//static Script* CreateScriptByName(std::string name, GameObject GO);
	};
}