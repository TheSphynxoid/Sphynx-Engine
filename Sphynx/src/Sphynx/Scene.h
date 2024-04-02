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
		bool IsCurrent = false;
		std::string SceneName = "Scene";
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
		//GameObject GetGameObject(std::string name);
		void Start();
		void Update();

		//TODO: Maybe make these take and return Camera not a pointer to it.

		Camera* GetPrimaryCamera() { 
			return PrimaryCamera;
		};
		void SetPrimaryCamera(Camera* cam) {
			PrimaryCameraObject = *cam->GetGameObject();
			PrimaryCamera = cam;
		}
		GameObject& GetPrimaryCameraObject() {
			return PrimaryCameraObject;
		}
		friend class SceneManager;
	};
}
