#pragma once
#include "Core/Scripting/ScriptingBackend.h"
#include "CsScript.h"

#define SphynxScripting Sphynx::Core::Scripting

struct _MonoClass;

typedef _MonoClass MonoClass;

namespace Sphynx::Mono {
	class MonoRuntime {
	private:
		static inline bool isAlive;
		static inline std::string GameAssemblyPath;
		static inline std::unordered_map<std::string, MonoClass*> CompNames;
		static inline std::unordered_map<std::string, CsScript> CachedScripts;
		static void AddManagedComponent(MonoClass* Object, std::string Fullname);
		static void ReadClassesMetadata();
	public:
		static void Initialize(std::string AssemblyPath);
		static void Shutdown();

		static inline bool IsAlive() { return isAlive; }

		static void ReloadGameAssembly();
		static inline void SetGameAssemblyPath(std::string path) { GameAssemblyPath = path; };

		static MonoClass* GetCommonType(std::string name);

		//Calls the default constructor.
		static MonoObject* CreateObject(MonoClass* klass);

		static SphynxScripting::Script* CreateScript(const char* path, GameObject* GO);
		static SphynxScripting::Script* CreateScriptByName(std::string name);

	};
}
#undef SphynxScripting