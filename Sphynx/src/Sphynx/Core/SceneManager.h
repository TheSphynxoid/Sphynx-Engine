#pragma once
#include "Scene.h"

namespace Sphynx::Core {
	class SceneManager final
	{
	private:
		static Scene CurrScene;
		static int SceneIndex;
		typedef std::list<Scene> Scenes;
		static Scenes GameScenes;
	public:
		static Scene& GetScene() {
			return CurrScene;
		};
		static void AddScene(Scene* scene) { 
			GameScenes.push_back(*scene); 
		};
		static void SetScene(Scene& scene);
		static void GoNextScene();
		static void Start();
		static void Update();
		static void UpdateCamera();
	};
}
