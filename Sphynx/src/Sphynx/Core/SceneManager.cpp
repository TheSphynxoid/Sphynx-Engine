#include "pch.h"
#include "SceneManager.h"
#include "Events/SceneEvents.h"

using namespace Sphynx::Core;

Scene SceneManager::CurrScene;
SceneManager::Scenes SceneManager::GameScenes;

void Sphynx::Core::SceneManager::SetScene(Scene& scene)
{
	CurrScene = scene;
	Start();
}

void Sphynx::Core::SceneManager::GoNextScene()
{
	CurrScene = *CurrScene.NextScene;
	Start();
}

void Sphynx::Core::SceneManager::Start()
{
	CurrScene.Start();
	Events::GlobalEventSystem::GetInstance()->QueueEvent<Sphynx::Events::OnSceneStart>(Sphynx::Events::OnSceneStart(CurrScene));
}

void Sphynx::Core::SceneManager::Update()
{
	CurrScene.Update();
}
