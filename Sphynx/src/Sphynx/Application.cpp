#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "SpTime.h"
#include "Pointer.h"
#include "Core/Graphics/Window.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "glm/gtx/quaternion.hpp"
#include "Scene.h"
#include "Core/SceneManager.h"
#include "Core/Graphics/Pipeline/FrameBuffer.h"
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

Sphynx::Application::Application()
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

void Sphynx::Application::Run()
{
	Events::GlobalEventSystem::GetInstance()->DispatchImmediate<Events::OnApplicationStart>(Events::OnApplicationStart());
	threadpool.Start(this);
	Input::Init();
	Start();
	SceneManager::Start();
	GameObject sq = GameObject::CreatePrimitive(Primitives::Cube, "Cube");
	sq.GetTransform()->Translate({ 0,0,-3 });
	SceneManager::GetScene().AddGameObject(&sq);
	Time::Start();
	int i = 0;
	while (AppAlive) {
		Update();
		Events::GlobalEventSystem::GetInstance()->DispatchImmediate<Events::OnApplicationUpdate>(Events::OnApplicationUpdate());
		//Events
		Events::GlobalEventSystem::GetInstance()->Dispatch();
		eventSystem.Dispatch();
		for (auto& es : EventSystemArray) {
			es->Dispatch();
		}
		if (MainWindow->IsAlive()) {
			MainWindow->GetRenderer()->Clear();
			SceneManager::Update();
			//Imgui.
			Imgui::OnOverlayUpdate();
			//Buffer Swap
			MainWindow->Update();
		}
		Time::Update();
	}
	Imgui::Shutdown();
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
		Imgui::Start();
		//Imgui::AddOverlayWindow(new DebugWindow());
		MainWindow->Start();
	}
	else {
		Core_Warn("A Window Is Already Open.");
	}
	Events::GlobalEventSystem::GetInstance()->DispatchImmediate(Events::OnWindowOpen(MainWindow));
	return MainWindow;
}