#pragma once
#include "Core/Scripting/ScriptingBackend.h"
#include "CsScript.h"

#define SphynxScripting Sphynx::Core::Scripting

struct _MonoClass;

typedef _MonoClass MonoClass;

namespace Sphynx::Mono {
	class MonoRuntime : public SphynxScripting::ScriptingBackend {
	private:
		bool isAlive;
		static std::unordered_map<std::string, MonoClass*> CompNames;
		static std::unordered_map<std::string, CsScript> CachedScripts;
		void AddManagedComponent(MonoClass* Object, std::string Fullname);
	public:
		MonoRuntime();
		~MonoRuntime();
		MonoRuntime(std::string AssemblyPath);
		// Inherited via ScriptingBackend
		virtual void Start() override;
		virtual void Update() override;
		virtual void Shutdown() override;

		inline bool IsAlive() { return isAlive; }

		virtual SphynxScripting::Script* CreateScript(const char* path, GameObject GO) override;
		virtual SphynxScripting::Script* CreateScriptByName(std::string name, GameObject GO) override;

	};
}
#undef SphynxScripting