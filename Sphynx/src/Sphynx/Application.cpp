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
#undef GetApplication
using namespace Sphynx;
using namespace Sphynx::Core;

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
	scriptingEngine = Core::Scripting::ScriptingEngine();
	scriptingEngine.Start(this);
	MainApplication = this;
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
		Core_Info("MovementComp Attached to {0}",parent->GetID());
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
	eventSystem.DispatchImmediate<Events::OnApplicationStart>(Events::OnApplicationStart());
	threadpool.Start(this);
	Camera::PrimaryCamera->GetTransform()->Translate(glm::vec3(0.0f, 0.0f, -3.0f));
	auto Square = GameObject::CreatePrimitive(Primitives::Cube);
	auto Square2 = GameObject::CreatePrimitive(Primitives::Cube);
	Square2.GetTransform()->Translate(glm::vec3(3.0f, 0.0f, 0.0f));
	Square.GetTransform()->Translate(glm::vec3(-3.0f, 0.0f, 0.0f));
	Square.GetTransform()->Rotate(30, glm::vec3(0.0f, 1.0f, 0.0f));
	Camera::PrimaryCamera->AddComponent<MovementComp>();
	Input::Init();
	Start();
	Time::Start();
	Camera::PrimaryCamera->GetTransform()->Translate(glm::vec3(0.0f, 0.0f, -3.0f));
	auto Square = GameObject::CreatePrimitive(Primitives::Cube);
	auto Square2 = GameObject::CreatePrimitive(Primitives::Cube);
	Square2.GetTransform()->Translate(glm::vec3(3.0f, 0.0f, 0.0f));
	Square.GetTransform()->Translate(glm::vec3(-3.0f, 0.0f, 0.0f));
	Square.GetTransform()->Rotate(30, glm::vec3(0.0f, 1.0f, 0.0f));
	Camera::PrimaryCamera->AddComponent<MovementComp>();
	Input::Init();
	Start();
	Time::Start();
	while (AppAlive) {
		Update();
		eventSystem.DispatchImmediate<Events::OnApplicationUpdate>(Events::OnApplicationUpdate());
		//Events
		Events::GlobalEventSystem::GetInstance()->Dispatch();
		eventSystem.Dispatch();
		for (auto& es : EventSystemArray) {
			es->Dispatch();
		}
		Square.Update();
		Square2.Update();
		Camera::PrimaryCamera->Update();
		if (MainWindow != NULL) {
			if (MainWindow->IsAlive()) {
				MainWindow->Update();
			}
		}
		Time::Update();
	}
	imgui.Shutdown();
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
			EventSystemArray.remove_if([&,ptr](Pointer<Events::EventSystem>& p)->bool {return ptr.GetRaw() == p.GetRaw(); });
			ptr.Release();
			delete &ptr;
		}
	}
}

Sphynx::Core::IWindow* Sphynx::Application::CreateMainWindow(std::unique_ptr<Core::IWindow>&& window)
{
	if (!MainWindow) {
		//Pointer Stolen!
		Core::IWindow* obj = window.release();
		MainWindow = obj;
		imgui.Start(this);
		imgui.AddOverlayWindow(new DebugWindow(this));
		MainWindow->Start();
	}
	else {
		Core_Warn("A Window Is Already Open.");
	}
	eventSystem.DispatchImmediate(Events::OnWindowOpen(MainWindow));
	return MainWindow;
}