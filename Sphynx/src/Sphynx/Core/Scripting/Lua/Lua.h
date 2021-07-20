#pragma once
#include "Delegate.h"
#undef LoadString
#define Dump(L) Lua::DumpStack(L);

struct lua_State;
struct lua_Debug;
namespace Sphynx {
	class Application;
}
namespace Sphynx::Core::Scripting {
	class Lua final
	{
	private:
		lua_State* State;
	public:
		//Signature for hook functions.
		typedef void (*Hook) (lua_State* L, lua_Debug* ar);
		//Signature for functions to be called by lua.
		typedef void (*Lua_CFunc)(lua_State* L);
		//Create a lua Instance.
		explicit Lua(Application* app);
		//Creates A Lua Object based on a lua instance.
		explicit Lua(lua_State* state) : State(state) {};
		//Destructor.
		~Lua();
		//Closes The Lua Instance.
		void Close();
		//Creates a new lua thread.
		Lua* CreateThread();
		//Executes the Given file (pcall)
		void ExecuteFile(const char* path);
		//Executes the given string (pcall)
		void ExecuteString(const char* code);
		//Load File
		void LoadFile(const char* path);
		//Load String
		void LoadString(const char* code);
		//Adds Script to be update (onupdate). it executes the file
		void AddScript(const char* str, bool IsFile);
		//Raises a Lua Error
		void RaiseError(const char* fmt, ...);
		//Registers a C++ function in lua
		void RegisterFunction(Lua_CFunc func, const char* name);
		//Prints the stack.
		void DumpStack();
		//Prints the stack.
		static void DumpStack(lua_State* State);
		//Should Lua Kill The Process At Panic ? if yes should it let the debugger run or not. 
		static void SetAtPanicKillSwitch(bool b, bool debug);
		//Not Implemented.
		const char* GetErrorString();
		//Convertion to lua_State (Lua instance).
		explicit operator lua_State* ();
	};
}