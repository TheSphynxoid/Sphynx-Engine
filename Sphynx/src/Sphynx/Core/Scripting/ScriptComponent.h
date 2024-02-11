#pragma once
#include "Component.h"
#include "Script.h"

namespace Sphynx::Mono {
	class GameObjectWrapper;
}

namespace Sphynx::Core {
	class ScriptComponent final : public Component 
	{
	private:
		Scripting::Script* script;
		Mono::GameObjectWrapper* GOWrapper;
	public:
		CompImpl(ScriptComponent);
		virtual Component* CopyInstance() {
			auto copy = new ScriptComponent(script->GetName());
			copy->script = script->Copy();
			return copy;
		}
		ScriptComponent(std::string scriptName);

		// Inherited via Component
		virtual void Start() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void OnComponentAttach(GameObject* parent) override;
		virtual void OnComponentDetach() override;
	};
}