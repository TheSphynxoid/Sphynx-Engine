#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Core/Graphics/Platform/GLWindow.h"
#include "SpTime.h"
#include "Core/Pointer.h"

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
	}
	GLWindow::TerminateGLFW();
	imgui.Shutdown();
	time.Shutdown();
}

Events::EventSystem Sphynx::Application::RequestNewEventSystem()
{
	auto e = std::unique_ptr<Events::EventSystem>();
	std::string s;
	//EventArray.push_back(e);
	return *e;
}

void Sphynx::Application::DeleteEventSystem(Events::EventSystem e)
{
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
		RemoveExtraWindow(win);
	}
}

void Sphynx::Application::AddExtraWindow(std::unique_ptr<Core::IWindow>&& window)
{
	//Ours now!
	Core::IWindow* obj = window.release();
	ExtraWindows.push_back(obj);
}

void Sphynx::Application::RemoveExtraWindow(Core::IWindow* window)
{
	//It Will Close the window whether it's in the list or not.
	//window->Close();
	ExtraWindows.remove(window);
}
