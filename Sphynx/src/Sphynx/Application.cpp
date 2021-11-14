#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "SpTime.h"
#include "Pointer.h"
#include "Core/Graphics/Window.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "glm/gtx/quaternion.hpp"
#include "GameObject.h"
#include "Camera.h"
#include "Core/MeshRenderer.h"
#include "Core/Scripting/AsScript.h"
#include "Core/Pool.h"
#undef GetApplication
#undef GetMainWindow

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Core::Graphics;

Application* MainApplication;

//void Sphynx::Application::HandleWindowClose(Events::OnWindowClose& e)
//{
//	e.GetWindow()->Close();
//}

Sphynx::Application::Application() : imgui(Imgui())
{
	eventSystem = Events::EventSystem();
	//eventSystem.Subscribe<Application, Events::OnWindowClose>(this, &Application::HandleWindowClose);
#if defined(DEBUG)
	Events::GlobalEventSystem::GetInstance()->Subscribe<Application, OnLog>(this, &Application::StdLog);
#endif
	MainApplication = this;
	this->CreateMainWindow(IWindow::Create(this));
	Scripting::ScriptingEngine::InitScripting();
}

Application* Sphynx::Application::GetApplication()
{
	return MainApplication;
}

Sphynx::Application::~Application()
{
	this->eventSystem.QueueEvent<Events::OnApplicationClose>(Events::OnApplicationClose());
}

class MovementComp : public Sphynx::Component {
	// Inherited via Component
	virtual void OnComponentAttach(GameObject* parent) override
	{
		Core_Info("MovementComp Attached to {0}", parent->GetID());
	}
	virtual void OnComponentDetach() override
	{
	}
	virtual void Update() override
	{
		if (Input::IsKeyPressed(Keys::Up))
			GetTransform()->Translate(glm::vec3(0.0f, 0.5f * Time::GetDeltaTime(), 0.0f));
		if (Input::IsKeyPressed(Keys::Down))
			GetTransform()->Translate(glm::vec3(0.0f, -0.5f * Time::GetDeltaTime(), 0.0f));
		if (Input::IsKeyPressed(Keys::Left))
			GetTransform()->Rotate(60 * Time::GetDeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		if (Input::IsKeyPressed(Keys::Right))
			GetTransform()->Rotate(-60 * Time::GetDeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
};

void Sphynx::Application::Run()
{
	
	Camera::PrimaryCamera->AddComponent<Camera>();
	Events::GlobalEventSystem::GetInstance()->DispatchImmediate<Events::OnApplicationStart>(Events::OnApplicationStart());
	threadpool.Start(this);
	Camera::PrimaryCamera->GetTransform()->Translate(glm::vec3(0.0f, 0.0f, -3.0f));
	auto Square = GameObject::CreatePrimitive(Primitives::Cube);
	auto Square2 = GameObject::CreatePrimitive(Primitives::Plane);
	Square2.GetTransform()->Translate(glm::vec3(3.0f, 0.0f, 0.0f));
	Square.GetTransform()->Rotate(30, glm::vec3(0.0f, 1.0f, 0.0f));
	Square.AddComponent<Sphynx::Core::Scripting::AsScript>("AsScript.as","TestModule");
	Square.AddComponent<MovementComp>();
	Sphynx::Pool lp = Sphynx::Pool<int>();
	lp.push(5);
	lp.push(12);
	Input::Init();
	Start();
	Time::Start();
	while (AppAlive) {
		Update();
		Events::GlobalEventSystem::GetInstance()->DispatchImmediate<Events::OnApplicationUpdate>(Events::OnApplicationUpdate());
		//Events
		Events::GlobalEventSystem::GetInstance()->Dispatch();
		eventSystem.Dispatch();
		for (auto& es : EventSystemArray) {
			es->Dispatch();
		}
		Square.Update();
		Square2.Update();
		Camera::PrimaryCamera->Update();
		if (MainWindow->IsAlive()) {
			MainWindow->Update();
		}
		Time::Update();
	}
	imgui.Shutdown();
	threadpool.Stop();
}

Events::EventSystem Sphynx::Application::RequestNewEventSystem()
{
	Pointer<Events::EventSystem>* e = AllocatePointer<Events::EventSystem>();
	EventSystemArray.push_back(*e);
	e->SetDestroyCallBack<Application>(Delegate<void, Application, Events::EventSystem&>(this, &Application::DeleteEventSystem));
	return *e->GetRaw();
}

void Sphynx::Application::DeleteEventSystem(Events::EventSystem& e)
{
	for (auto& ptr : EventSystemArray) {
		if (ptr.GetRaw() == &e) {
			EventSystemArray.remove_if([&, ptr](Pointer<Events::EventSystem>& p)->bool {return ptr.GetRaw() == p.GetRaw(); });
			ptr.Release();
			delete& ptr;
		}
	}
}

Sphynx::Core::IWindow* Sphynx::Application::CreateMainWindow(Core::IWindow* window)
{
	if (!MainWindow) {
		MainWindow = window;
		imgui.Start(this);
		imgui.AddOverlayWindow(new DebugWindow(this));
		MainWindow->Start();
	}
	else {
		Core_Warn("A Window Is Already Open.");
	}
	Events::GlobalEventSystem::GetInstance()->DispatchImmediate(Events::OnWindowOpen(MainWindow));
	return MainWindow;
}