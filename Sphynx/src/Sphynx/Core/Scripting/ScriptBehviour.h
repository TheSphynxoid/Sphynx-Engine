#pragma once

class asITypeInfo;

namespace Sphynx {
	class GameObject;
}
namespace Sphynx::Core::Scripting {
	class ScriptBehviour final
	{
	private:
		//int RefCount = 1;
		const char* Name = "NULL";
		GameObject* Holder = nullptr;
		void* ScriptObject = NULL;
		asITypeInfo* TypeInfo = nullptr;
		ScriptBehviour() {};
		ScriptBehviour(const char* name, GameObject* holder, void* scriptObject, asITypeInfo* typeinfo);
	public:
		const std::string& GetName() noexcept{
			return Name;
		}
		GameObject* GetGameObject()const noexcept { return Holder; };

		friend class AsScript;
		friend class AngelScript;
	};
}
