#include "pch.h"
#include "Application.h"
#include "Event/Event.h"
#include "Event/ApplicationEvents.h"

Sphynx::Application::Application() 
{
	eventSystem = Events::EventSystem();
}

Sphynx::Application::~Application()
{
	this->eventSystem.Dispatch<Events::OnApplicationClose>(Events::OnApplicationClose());
}

void Sphynx::Application::Run()
{
	eventSystem.Dispatch<Events::OnApplicationStart>(Events::OnApplicationStart());
	while (true);
}
