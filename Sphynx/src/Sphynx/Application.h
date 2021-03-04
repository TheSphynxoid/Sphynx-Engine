#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "SpTime.h"
#include "Core/Imgui.h"

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
	class Application
	{
	protected:
		Events::EventSystem eventSystem;
		Core::Imgui imgui;
	private:
		Core::IWindow* window;
		Sphynx::Time time;
	public:
		Application();
		virtual ~Application();
		void Run();
		Events::EventSystem* GetAppEventSystem()noexcept { return &eventSystem; };
		Core::IWindow* GetAppWindow()noexcept { return window; };
		Sphynx::Time* GetTimeObject();
	};
	//To be defined in a client
	Application* CreateApplication();
}
