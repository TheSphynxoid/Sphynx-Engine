#pragma once
#include "Module.h"
#include "Events/WindowEvents.h"
#include "SpTime.h"
#include "Core/System.h"
#include <list>


namespace Sphynx::Core {
	class IWindow;

	//Base Imgui Windows
	class IOverlayWindow {
	private:
	public:
		IOverlayWindow() = default;
		virtual void Draw() = 0;
	};
	class DemoWindow : public IOverlayWindow {
	public:
		void Draw()override;
	};
	class AboutWindow : public IOverlayWindow {
		void Draw()override;
	};
	class Imgui final : public Module{
	private:
		Application* App;
		IWindow* window = NULL;
		std::list<IOverlayWindow*> Overlays;
		Time& time = Time();
	public:
		void Start(Application* app)override;
		void OnOverlayUpdate(Events::OnOverlayUpdate& e);
		void Update()override;
		void Shutdown()override;
		//We Can Also Use the event System. Instead of holding window we just dispatch an event.

		//Adds a window to the draw list.
		void AddOverlayWindow(IOverlayWindow* Window);
		//Remove the window from the list.
		void RemoveOverlayWindow(IOverlayWindow* Window);
	};
}
