#pragma once

struct lua_State;

namespace Sphynx {
	class Application;
}

namespace Sphynx::Core::Scripting {
	class LuaWindow {
	private:
		static void CreateWindowMeta(lua_State* L);
		static void CreateBoundsMeta(lua_State* L);
	public:
		static void CreateModule(lua_State* L,Application* app);
	};
}