#pragma once
#include "GameObject.h"
#include "GameObjectWrapper.h"
#include "../CsScript.h"

namespace Sphynx::Mono {
	class ScriptManager final {
	private:
		//Not sure if this is problematic if there is erasures.
		static inline std::unordered_map<size_t, GameObjectWrapper*> GOWrapperRegistry;
	public:
		static GameObjectWrapper* CreateGameObjectWrapper(GameObject* go) {
			GOWrapperRegistry[go->GetID()] = new GameObjectWrapper(go);
			return GOWrapperRegistry[go->GetID()];
		};
		static GameObjectWrapper* GetGameobjectWrapper(GameObject* go) {
			if (GOWrapperRegistry[go->GetID()] == nullptr) {
				return CreateGameObjectWrapper(go);
			}
			else {
				return GOWrapperRegistry[go->GetID()];
			}
		};
	};
}
