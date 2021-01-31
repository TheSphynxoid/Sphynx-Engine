#pragma once

#include "Core.h"
#include "Event/Event.h"

namespace Sphynx {
	class Application
	{
		Events::EventSystem eventSystem;
	public:
		Application();
		virtual ~Application();
		void Run();
	};
	//To be defined in a client
	Application* CreateApplication();
}

