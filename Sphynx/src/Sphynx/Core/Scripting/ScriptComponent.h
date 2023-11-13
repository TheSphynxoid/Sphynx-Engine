#pragma once
#include "Component.h"
#include "Script.h"


namespace Sphynx::Core {
	class ScriptComponent : public Component 
	{
	private:
		Scripting::Script* script;
	public:
		ScriptComponent(std::string name);

		// Inherited via Component
		void OnComponentAttach(GameObject* parent) override;
		const char* GetName() override;
	};
}