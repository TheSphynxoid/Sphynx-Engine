#pragma once
#include "Core/Module.h"
#include "angelscript.h"
#include "Script.h"
#include "ScriptBehviour.h"
#include <scriptbuilder/scriptbuilder.h>
#include "Events/WindowEvents.h"
#include "Events/ApplicationEvents.h"

namespace Sphynx::Core::Scripting {
	//How do i add the Concept of Component and interaction between Scripts.
	class AsScript final : public Script {
	private:
		virtual void OnComponentAttach(GameObject* parent) override;
		virtual void OnComponentDetach() override;
		asIScriptModule* Module = nullptr;
		asIScriptContext* Context = nullptr;
		asIScriptFunction* StartFunc = nullptr;
		asIScriptFunction* UpdateFunc = nullptr;
		ScriptBehaviour scriptBehaviour;
		//Register all the types if more then one are in files and find a way to choose.
		std::list<asITypeInfo*> FileTypes;
		const char* Path;
		const char* SectionName;
		const char* ModuleName;

		asIScriptEngine* GetEngine();
	public:
		AsScript(const char* path, const char* moduleName);
		~AsScript();
		virtual void Update() override;
		virtual void Run() override;
		const ScriptBehaviour& GetScriptBehaviour() const{
			return scriptBehaviour;
		}
		const char* GetSectionName() {
			return SectionName;
		}
		friend class AngelScript;
	};
	class AngelScript final{
	private:
		asIScriptEngine* Engine;
		CScriptBuilder builder;
		void OnAppStart(Events::OnApplicationStart& e);
		void OnWindowOpen(Events::OnWindowOpen& e);
	public:
		AngelScript();
		~AngelScript();
		void CreateScript(AsScript* script, std::string& code, const char* sectionName, const char* ModuleName);
		void CreateScript(AsScript* script, const char* path, const char* ModuleName);
		asITypeInfo* GetSystemClassTypeInfo(const char* name);
		asIScriptObject* CreateScriptObject(const asITypeInfo* type, ScriptBehaviour* Behaviour);
		//Reloads the scripts and destroys the old state
		void ReloadScript(AsScript* script);
		
		friend asIScriptEngine* AsScript::GetEngine();
	};
}
