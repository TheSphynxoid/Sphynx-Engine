#pragma once
#include "WindowEvents.h"
#include "Core/Module.h"

namespace Sphynx::Core {
	class Imgui;
}

namespace Sphynx::Events {
	struct OnOverlayUpdate : public WindowEvent {
		OnOverlayUpdate(Sphynx::Core::IWindow* win) : WindowEvent(win) {};
	};
	struct OnOverlayDelete : public Event {

	};
	struct OnOverlayModuleDown : public Event {
		OnOverlayModuleDown() {};
	};
	//struct OnImGuiContextCreation : public Event{
	//private:
	//	Core::Imgui* inst;
	//public:
	//	OnImGuiContextCreation(Core::Imgui* imgui) { inst = imgui; };
	//	Core::Imgui* GetImgui() { return inst; };
	//};

}