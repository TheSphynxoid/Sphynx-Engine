#pragma once
#include "Event.h"

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
}

namespace Sphynx::Events {
	struct WindowEvent : public Event {
	protected:
		Sphynx::Core::IWindow* window;
		WindowEvent() = delete;
		WindowEvent(Sphynx::Core::IWindow* win) : window(win) {};
		Sphynx::Core::IWindow* GetWindow() { return window; };
	};
	struct OnWindowResize :public WindowEvent {
		int Width, Height;
		OnWindowResize(Sphynx::Core::IWindow* win, int width, int height) :Width(width), Height(height), WindowEvent(win) {};
	};
	struct OnWindowUpdate : public WindowEvent {
	public:
		OnWindowUpdate(Sphynx::Core::IWindow* win) : WindowEvent(win) {};
	};
	struct OnWindowClose : public WindowEvent {
		OnWindowClose(Sphynx::Core::IWindow* win) : WindowEvent(win) {};
	};
	struct OnOverlayUpdate : public WindowEvent {
		OnOverlayUpdate(Sphynx::Core::IWindow* win) : WindowEvent(win) {};
	};
	struct OnOverlayDelete : Event{

	};
	struct OnOverlayModuleDown : Event {
		OnOverlayModuleDown() {};
	};
}