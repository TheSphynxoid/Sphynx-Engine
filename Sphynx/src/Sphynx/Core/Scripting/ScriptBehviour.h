#pragma once

class asITypeInfo;
class asIScriptObject;

namespace Sphynx {
	class GameObject;
}
namespace Sphynx::Core::Scripting {
	//TODO: Refactoring.
	class ScriptBehaviour final
	{
	private:
		//int RefCount = 1;
		const char* Name = "NULL";
		GameObject* Holder = nullptr;
		//THIS
		asIScriptObject* ScriptObject = NULL;
		//AND THIS.
		const asITypeInfo* TypeInfo = nullptr;
		ScriptBehaviour() {};
		ScriptBehaviour(GameObject* holder, asIScriptObject* scriptObject, asITypeInfo* typeinfo);
	public:
		ScriptBehaviour(GameObject* holder) : ScriptBehaviour(holder, nullptr, nullptr) {};
		~ScriptBehaviour() = default;
		const std::string& GetName() noexcept{
			return Name;
		}
		GameObject* GetGameObject()const noexcept { return Holder; };
		const asITypeInfo* GetTypeInfo()const noexcept { return TypeInfo; };
		friend class AsScript;
		friend class AngelScript;
	};
}
