#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Core/Platform/GLWindow.h"

using namespace Sphynx;
using namespace Sphynx::Core;

Sphynx::Application::Application() 
{
	eventSystem = Events::EventSystem();
	window = new GLWindow(this, Bounds(1024, 576),"Cool Window");
}

Sphynx::Application::~Application()
{
	this->eventSystem.Dispatch<Events::OnApplicationClose>(Events::OnApplicationClose());
}

void Sphynx::Application::Run()
{
	eventSystem.Dispatch<Events::OnApplicationStart>(Events::OnApplicationStart());
	while (true) {
		window->Update();
	}
}
