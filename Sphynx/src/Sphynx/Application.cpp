#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Core/Window/Platform/GLWindow.h"

using namespace Sphynx;
using namespace Sphynx::Core;

void UpdateTest(Events::OnApplicationUpdate& e) {
	Core_Info("Called");
}

Sphynx::Application::Application() 
{
	eventSystem = Events::EventSystem();
	window = new GLWindow(this, Bounds(1024, 576),"Cool Window");
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
		window->Update();
	}
}
