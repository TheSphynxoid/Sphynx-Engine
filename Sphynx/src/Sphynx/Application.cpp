#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Core/Window/Platform/GLWindow.h"
#include "SpTime.h"

using namespace Sphynx;
using namespace Sphynx::Core;

void UpdateTest(Events::OnApplicationUpdate& e) {
	Core_Info("Called");
}

Sphynx::Application::Application() : time(Time()), imgui(Imgui())
{
	time.Start(this);
	eventSystem = Events::EventSystem();
	window = new GLWindow(this, Bounds(1024, 576),"Then and There");
	imgui.Start(this);
	imgui.AddOverlayWindow(new AboutWindow());
	imgui.AddOverlayWindow(new DemoWindow());
	window->Start();
	eventSystem.Subscribe<Events::OnApplicationUpdate>(&UpdateTest);
}

Sphynx::Application::~Application()
{
	this->eventSystem.Dispatch<Events::OnApplicationClose>(Events::OnApplicationClose());
}
void Sphynx::Application::Run()
{
	eventSystem.Dispatch<Events::OnApplicationStart>(Events::OnApplicationStart());
	while (window->IsAlive()) {
		time.Update();
		window->Update();

		//imgui.Update();
	}
	imgui.Shutdown();
	time.Shutdown();
}

Time& Sphynx::Application::GetTimeObject()
{
	// TODO: insert return statement here
	return time;
}
