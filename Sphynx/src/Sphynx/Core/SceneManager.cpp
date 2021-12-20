#include "pch.h"
#include "SceneManager.h"

using namespace Sphynx::Core;

Scene SceneManager::CurrScene;
SceneManager::Scenes SceneManager::GameScenes;

void Sphynx::Core::SceneManager::SetScene(Scene& scene)
{
}

void Sphynx::Core::SceneManager::GoNextScene()
{
}

void Sphynx::Core::SceneManager::Start()
{
	CurrScene.Start();
}

void Sphynx::Core::SceneManager::Update()
{
	CurrScene.Update();
}

void Sphynx::Core::SceneManager::UpdateCamera()
{
	CurrScene.PrimaryCameraObject.Update();
}
