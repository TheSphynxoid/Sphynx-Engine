#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "SpTime.h"
#include "Pointer.h"
#include "Core/Graphics/Window.h"

using namespace Sphynx;
using namespace Sphynx::Core;

void Sphynx::Application::HandleWindowClose(Events::OnWindowClose& e)
{
	e.GetWindow()->Close();
}

Sphynx::Application::Application() : imgui(Imgui())
{
	eventSystem = Events::EventSystem();
	eventSystem.Subscribe<Application, Events::OnWindowClose>(this, &Application::HandleWindowClose);
#if defined(DEBUG)
	Events::GlobalEventSystem::GetInstance()->Subscribe<Application, OnLog>(this, &Application::StdLog);
#endif
	scriptingEngine = Core::Scripting::ScriptingEngine();
	scriptingEngine.Start(this);
	scriptingEngine.GetLua().ExecuteFile("Test.lua");
	if (MainWindow) {
		imgui.Start(this);
		imgui.AddOverlayWindow(new DebugWindow(this));
		//imgui.AddOverlayWindow(new AboutWindow());
		//imgui.AddOverlayWindow(new DemoWindow());
		MainWindow->Start();
	}
}

Sphynx::Application::~Application()
{
	this->eventSystem.QueueEvent<Events::OnApplicationClose>(Events::OnApplicationClose());
}

void Sphynx::Application::Run()
{
	eventSystem.DispatchImmediate<Events::OnApplicationStart>(Events::OnApplicationStart());
	threadpool.Start(this);
	//threadpool.Submit(Delegate<void,void>(&test));
	//threadpool.Submit(Delegate<void, void>(&test2));
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
		UpdateWindow();
		Time::Update();
	}
	threadpool.Stop();
	imgui.Shutdown();	
}

void Sphynx::Application::UpdateWindow()
{
	if (MainWindow != NULL) {
		if (MainWindow->IsAlive()) {
			MainWindow->Update();
			if (ExtraWindow)ExtraWindow->Update();
		}
	}
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
			//EventSystemArray.remove(ptr);
			EventSystemArray.remove_if([&,ptr](Pointer<Events::EventSystem>& p)->bool {return ptr.GetRaw() == p.GetRaw(); });
			ptr.Release();
			delete &ptr;
		}
	}
}

Sphynx::Core::IWindow* Sphynx::Application::CreateMainWindow(std::unique_ptr<Core::IWindow>&& window)
{
	if (!MainWindow) {
		//Stolen!
		Core::IWindow* obj = window.release();
		MainWindow = obj;
		imgui.Start(this);
		imgui.AddOverlayWindow(new DebugWindow(this));
		//imgui.AddOverlayWindow(new AboutWindow());
		//imgui.AddOverlayWindow(new DemoWindow());
		MainWindow->Start();
	}
	else {
		Core_Warn("Non Fatal : Overwriting Main Window is not allowed. Close it and then Open a new one.");
	}
	return MainWindow;
}

void Sphynx::Application::CloseAllWindows()
{
	imgui.Shutdown();
	if (MainWindow)MainWindow->Close();
	if (ExtraWindow)ExtraWindow->Close();
}

Core::IWindow* Sphynx::Application::CreateExtraWindow(std::unique_ptr<Core::IWindow>&& window)
{
	//Ours now!
	ExtraWindow = window.release();
	ExtraWindow->Start();
	return ExtraWindow;
}

void Sphynx::Application::CloseExtraWindow()
{
	ExtraWindow->Close();
	ExtraWindow = nullptr;
}
