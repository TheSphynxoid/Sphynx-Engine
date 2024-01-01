#include "pch.h"
#include "Scene.h"
#include "Camera.h"
#include "Core/MeshRenderer.h"
#include "UI/RectRenderer.h"
#include "Core/Graphics/Pipeline/FrameBuffer.h"

Sphynx::Core::Scene::Scene()
{
	//Events::GlobalEventSystem::GetInstance()->Subscribe<Scene, Events::OnWindowResize>(this, &Scene::OnFrameBufferResize);
}

void Sphynx::Core::Scene::OnFrameBufferResize(Events::OnWindowResize& e)
{
	//PrimaryCameraObject.GetComponent<UI::RectRenderer>()->GetMaterial()->SetTexture(PrimaryCamera->GetFrameBuffer()->GetColorAttachment(1), 0);
}

Sphynx::Core::Scene::~Scene()
{
	Core_Info("Scene Destroyed");
}

void Sphynx::Core::Scene::AddGameObject(GameObject* obj)
{
	GObjs.push_front(obj);
	obj->Start();
}

void Sphynx::Core::Scene::RemoveGameObject(GameObject* obj)
{
	GObjs.remove(obj);
	delete obj;
}

Sphynx::Core::Scene::GameObjects Sphynx::Core::Scene::GetGameObjects()
{
	return GObjs;
}

void Sphynx::Core::Scene::Start()
{
	PrimaryCameraObject.SetName("Camera");
	PrimaryCameraObject.AddComponent<Camera>();
	PrimaryCamera = PrimaryCameraObject.GetComponent<Camera>();
	GObjs.push_back(&PrimaryCameraObject);
	for (auto& go : GObjs) {
		go->Start();
	}
}

void Sphynx::Core::Scene::Update()
{
	for (auto& go : GObjs) {
		go->Update();
	}
}
