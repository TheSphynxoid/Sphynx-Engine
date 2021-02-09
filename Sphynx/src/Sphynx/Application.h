#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
	class Application
	{
	protected:
		Events::EventSystem eventSystem;
	private:
		Core::IWindow* window;
	public:
		Application();
		virtual ~Application();
		void Run();
		Events::EventSystem GetAppEventSystem() { return eventSystem; };
	};
	//To be defined in a client
	Application* CreateApplication();
}

