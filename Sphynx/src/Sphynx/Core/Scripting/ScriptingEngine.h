#pragma once
#include "Core/Module.h"
#include "Core/Scripting/Lua/Lua.h"
#include "Pointer.h"

namespace Sphynx::Core::Scripting {
	class ScriptingEngine final : public Module
	{
	private:
		Lua* LuaState = nullptr;
	public:
		// Inherited via Module
		virtual void Start(Application* app) override;
		virtual void Update() override;
		//Creates and Returns the Old Lua State if it exists.
		Lua* CreateLuaThread();
		inline Lua GetLua() { return *LuaState; };
	};
}