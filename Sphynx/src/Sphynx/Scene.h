#pragma once
#include "GameObject.h"

namespace Sphynx::Core {
	class Scene
	{
	private:
		typedef std::list<GameObject*> GameObjects;
		GameObjects GObjs = GameObjects();
		unsigned int SceneNum = 0;
		std::string SceneName = "Scene";
		bool IsCurrent = false;
		Scene* NextScene;
	public:
		Scene();
		~Scene();
		std::string& GetName() { 
			return SceneName; 
		};
		void SetName(const std::string& name) { 
			SceneName = name; 
		};
		void AddGameObject(GameObject* obj);
		void RemoveGameObject(GameObject* obj);
		GameObjects GetGameObjects();
		void Start();
		void Update();

		friend class SceneManager;
	};
}
