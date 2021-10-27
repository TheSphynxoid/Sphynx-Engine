#pragma once
#include "GameObject.h"
#include "Events/Event.h"

namespace Sphynx::Core {
	class Scene;
	struct SceneEvent : public Events::Event {
	protected:
		Scene* scene;
	public:
		SceneEvent(Scene* s) : scene(s) {};
		Scene* GetScene() { return scene; };
	};
	struct OnSceneObjectAdded final : public SceneEvent {
		GameObject* obj;
	public:
		OnSceneObjectAdded(Scene* s, GameObject* o) : SceneEvent(s), obj(o) {};
	};
	class Scene
	{
	private:
		typedef std::list<GameObject*> GameObjects;
		GameObjects _gameObjects;

	public:
		const GameObjects& GetGameObjects() { return _gameObjects; };
		void AddGameObject(GameObject* GO) 
		{
			_gameObjects.push_back(GO);
			Events::GlobalEventSystem::GetInstance()->QueueEvent<OnSceneObjectAdded>(OnSceneObjectAdded(this, GO));
		};
		void Update(){
			for (auto GO : _gameObjects) {
				if(GO->IsActive())
					GO->Update();
			};
		}
	};
}