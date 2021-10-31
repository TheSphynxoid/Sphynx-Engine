#pragma once
#include "Core/Module.h"
#include "angelscript.h"
#include "Script.h"
#include <scriptbuilder/scriptbuilder.h>
#include "Events/WindowEvents.h"
#include "Events/ApplicationEvents.h"

namespace Sphynx::Core::Scripting {
	class AsScript : public Script {
	private:
		virtual void OnComponentAttach(GameObject* parent) override;
		virtual void OnComponentDetach() override;
		asIScriptModule* Module = nullptr;
		asIScriptContext* Context = nullptr;
	public:
		AsScript(const char* path, const char* ModuleName);
		~AsScript();
		virtual void Update() override;
		virtual void Run() override;

		friend class AngelScript;
	};
	class AngelScript{
	private:
		asIScriptEngine* Engine;
		CScriptBuilder builder;
		void OnAppStart(Events::OnApplicationStart& e);
		void OnWindowOpen(Events::OnWindowOpen& e);
	public:
		AngelScript();
		~AngelScript();
		void CreateScript(AsScript* script, std::string& code, const char* SectionName, const char* ModuleName);
		void CreateScript(AsScript* script, const char* path, const char* ModuleName);
	};
}