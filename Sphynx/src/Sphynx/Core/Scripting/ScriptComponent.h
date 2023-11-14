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
		virtual void Start() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void OnComponentAttach(GameObject* parent) override;
		virtual void OnComponentDetach() override;
		const char* GetName() override;
	};
}