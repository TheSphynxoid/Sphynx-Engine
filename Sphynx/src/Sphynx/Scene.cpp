#include "pch.h"
#include "Scene.h"

Sphynx::Core::Scene::Scene()
{

}

Sphynx::Core::Scene::~Scene()
{
	Core_Info("Scene Destroyed");
}

void Sphynx::Core::Scene::AddGameObject(GameObject* obj)
{
	GObjs.push_back(obj);
}

void Sphynx::Core::Scene::RemoveGameObject(GameObject* obj)
{
}

Sphynx::Core::Scene::GameObjects Sphynx::Core::Scene::GetGameObjects()
{
	return GObjs;
}

void Sphynx::Core::Scene::Start()
{
}

void Sphynx::Core::Scene::Update()
{
	for (auto& go : GObjs) {
		go->Update();
	}
}
