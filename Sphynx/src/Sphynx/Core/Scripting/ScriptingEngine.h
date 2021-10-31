#pragma once
#include "Core/Module.h"
#include "Pointer.h"

namespace Sphynx::Core::Scripting {
	class AngelScript;
	class ScriptingEngine final
	{
	private:
		static AngelScript As;
	public:
		static void InitScripting();
		static AngelScript& GetAngelScript() { return As; };
	};
}