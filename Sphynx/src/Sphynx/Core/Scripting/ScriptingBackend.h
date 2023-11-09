#pragma once
#include "System.h"
#include "GameObject.h"

namespace Sphynx::Core::Scripting {
	class Script;
	class ScriptingBackend : public System {
	private:
	public:
		ScriptingBackend() = default;
		virtual ~ScriptingBackend() = default;

		virtual inline UpdateOn GetSystemUpdateOn() override { return UpdateOn::OnUpdate; };

		virtual Script* CreateScript(const char* path, GameObject GO) = 0;
		virtual Script* CreateScriptByName(std::string name, GameObject GO) = 0;
	};
}