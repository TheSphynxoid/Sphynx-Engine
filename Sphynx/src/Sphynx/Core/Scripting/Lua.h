#pragma once

struct lua_State;
struct lua_Debug;

namespace Sphynx::Core::Scripting {
	class Lua
	{
	private:
		lua_State* State;
		Lua(lua_State* state) : State(state) {};
	public:
		//Signature for hook functions
		typedef void (*Hook) (lua_State* L, lua_Debug* ar);
		typedef void (*Lua_CFunc)(lua_State* L);
		//Create a lua Instance
		Lua();
		~Lua();
		//Creates a new lua thread.
		Lua* CreateThread();
		//Executes the Given file (pcall)
		void ExecuteFile(const char* path);
		//Executes the given string (pcall)
		void ExecuteString(const char* code);
		//Raises a Lua Error
		void RaiseError(const char* fmt, ...);
		//Registers a C++ function in lua
		void RegisterFunction(Lua_CFunc func, const char* name);
		//Dumps and Prints the stack.
		void PrintStack();
		//Not Implemented.
		const char* GetErrorString();
	};
}