#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Core/Graphics/Platform/GLWindow.h"
#include "SpTime.h"
#include "Pointer.h"

using namespace Sphynx;
using namespace Sphynx::Core;

void Sphynx::Application::HandleWindowClose(Events::OnWindowClose& e)
{
	if (e.GetWindow() == MainWindow) {
		CloseMainWindow();
	}
	else {
		RemoveExtraWindow(e.GetWindow());
	}
}

Sphynx::Application::Application() : time(Time()), imgui(Imgui())
{
	time.Start(this);
	eventSystem = Events::EventSystem();
	eventSystem.Subscribe<Application, Events::OnWindowClose>(this, &Application::HandleWindowClose);
#if defined(DEBUG)
	Events::GlobalEventSystem::GetInstance()->Subscribe<Application, OnLog>(this, &Application::StdLog);
#endif
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
	while (AppAlive) {
		Update();
		eventSystem.DispatchImmediate<Events::OnApplicationUpdate>(Events::OnApplicationUpdate());
		time.Update();
		if (MainWindow != NULL) {
			if (MainWindow->IsAlive()) {
				MainWindow->Update();
				for (auto& win : ExtraWindows) {
					win->Update();
				}
			}
		}
		//Events
		Events::GlobalEventSystem::GetInstance()->Dispatch();
		eventSystem.Dispatch();
		for (auto& es : EventArray) {
			es->Dispatch();
		}
	}
	imgui.Shutdown();
	for (auto& win : ExtraWindows) {
		//A Closed Window doesn't exist.
		RemoveExtraWindow(win);
	}
	GLWindow::TerminateGLFW();
	time.Shutdown();
}

void DTest(int i) {
	std::cout << "wut " << i;
}

Events::EventSystem Sphynx::Application::RequestNewEventSystem()
{
	Pointer<Events::EventSystem>* e = AllocatePointer<Events::EventSystem>();
	EventArray.push_back(*e);
	e->SetDestroyCallBack<Application>(Delegate<void, Application, Events::EventSystem&>(this, &Application::DeleteEventSystem));
	return *e->GetRaw();
}

void Sphynx::Application::DeleteEventSystem(Events::EventSystem& e)
{
	for (auto& ptr : EventArray) {
		if (ptr.GetRaw() == &e) {
			//EventArray.remove(ptr);
			EventArray.remove_if([&,ptr](Pointer<Events::EventSystem>& p)->bool {return ptr.GetAddress() == p.GetAddress(); });
			ptr.Release();
			delete &ptr;
		}
	}
}

Time* Sphynx::Application::GetTimeObject()
{
	// TODO: insert return statement here
	return &time;
}

void Sphynx::Application::OpenMainWindow(std::unique_ptr<Core::IWindow>&& window)
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
}

void Sphynx::Application::CloseMainWindow()
{
	imgui.Shutdown();
	//HELL.
	auto placeholder = MainWindow;
	MainWindow = NULL;
	delete placeholder;
	//MainWindow->~IWindow();
}

void Sphynx::Application::CloseAllWindow()
{
	CloseMainWindow();
	for (auto& win : ExtraWindows) {
		//A Closed Window doesn't exist.
		//Issue: Closing a extra window Raises an event that Call RemoveExtraWindow with the same window.
		//I Don't know if it will cause problems.
		RemoveExtraWindow(win);
	}
}

void Sphynx::Application::AddExtraWindow(std::unique_ptr<Core::IWindow>&& window)
{
	//Ours now!
	Core::IWindow* obj = window.release();
	obj->Start();
	ExtraWindows.push_back(obj);
}

void Sphynx::Application::RemoveExtraWindow(Core::IWindow* window)
{
	ExtraWindows.remove(window);
}
