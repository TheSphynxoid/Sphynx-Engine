#pragma once
#include "Core/Scripting/ScriptingBackend.h"


namespace Sphynx::Mono {
	class MonoRuntime : public Sphynx::Core::Scripting::ScriptingBackend {
	private:
		bool isAlive;
	public:
		MonoRuntime();
		~MonoRuntime();
		MonoRuntime(std::string AssemblyPath);
		// Inherited via ScriptingBackend
		virtual void Start() override;
		virtual void Update() override;
		virtual void Shutdown() override;


		inline bool IsAlive() { return isAlive; }
	};
}