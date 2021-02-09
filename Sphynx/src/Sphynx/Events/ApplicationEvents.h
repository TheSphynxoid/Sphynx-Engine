#pragma once
#include "Event.h"
//#include "Core/Window.h"

namespace Sphynx::Core {
	class IWindow;
}

namespace Sphynx::Events {
	struct OnApplicationStart : public Event {
		OnApplicationStart() = default;
	};
	struct OnApplicationClose : public Event {
		OnApplicationClose() = default;
	};
	struct OnApplicationUpdate : public Event {

	};
	struct WindowEvent : public Event {
	protected:
		Sphynx::Core::IWindow* window;
		WindowEvent() = delete;
		WindowEvent(Sphynx::Core::IWindow* win) : window(win) {};
	};
	struct OnWindowResize :public WindowEvent {
		int Width, Height;
		OnWindowResize(Sphynx::Core::IWindow* win, int width, int height) :Width(width), Height(height), WindowEvent(win) {};
	};
	struct OnWindowUpdate : public WindowEvent{
	public:
		Sphynx::Core::IWindow* GetWindow() { return window; };
		OnWindowUpdate(Sphynx::Core::IWindow* win) : WindowEvent(win) {};
	};
}