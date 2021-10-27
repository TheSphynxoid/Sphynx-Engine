#pragma once
#include "Core/Module.h"
#include "Pointer.h"

namespace Sphynx::Core::Scripting {
	class ScriptingEngine final : public Module
	{
	public:
		// Inherited via Module
		virtual void Start(Application* app) override;
		virtual void Update() override;
		//Creates and Returns the Old Lua State if it exists.
	};
}