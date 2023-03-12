#pragma once
#include "Component.h"

namespace Sphynx::Core::Scripting {
	class Script : public Component {
		// Inherited via Component
		virtual void OnComponentAttach(GameObject* parent) = 0;
		virtual void OnComponentDetach() = 0;
		virtual void Update() = 0;

	public:
		virtual void Run() = 0;
	};
}