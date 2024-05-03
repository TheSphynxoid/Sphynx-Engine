#pragma once
#include "Core/Scripting/ScriptingBackend.h"
#include "CsScript.h"

#define SphynxScripting Sphynx::Core::Scripting

struct _MonoClass;
typedef _MonoClass MonoClass;
struct _MonoDomain;
typedef _MonoDomain MonoDomain;
struct _MonoImage;
typedef _MonoImage MonoImage;

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
		static MonoObject* CreateInitializedObject(MonoClass* klass);
		static MonoObject* CreateObject(MonoClass* klass);

		static MonoDomain* GetAppdomain()noexcept;
		static MonoDomain* GetCoreDomain()noexcept;

		static MonoImage* GetCoreAssemblyImage()noexcept;
		static MonoImage* GetUserAssemblyImage()noexcept;

		static SphynxScripting::Script* CreateScript(const char* path, GameObject* GO);
		static SphynxScripting::Script* CreateScriptByName(std::string name);
		static void DestroyScriptInstance(SphynxScripting::Script* s);
		static void ClearScriptFromCache(const std::string& name)noexcept;

	};
	void HandleException(MonoException* ex);
}
#undef SphynxScripting