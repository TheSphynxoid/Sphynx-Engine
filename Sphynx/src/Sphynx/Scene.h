#pragma once
#include "GameObject.h"
#include "Camera.h"

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
		GameObject PrimaryCameraObject;
		Camera* PrimaryCamera;

		void OnFrameBufferResize(Events::OnWindowResize& e);
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
		Camera* GetPrimaryCamera() { 
			return PrimaryCamera;
		};
		GameObject& GetPrimaryCameraObject() {
			return PrimaryCameraObject;
		}
		friend class SceneManager;
	};
}
