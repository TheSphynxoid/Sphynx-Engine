#pragma once
#include <sstream>

#include "Events/ApplicationEvents.h"
#include "Application.h"
#include "Core.h"

namespace Sphynx::Core {
	struct Coords {
		int x, y;
		Coords() : x(0), y(0) {};
		Coords(int _x, int _y) : x(_x), y(_y) {};
	};
	struct Bounds {
		int Height, Width;
		Bounds() : Height(0), Width(0) {};
		Bounds(int width, int height) : Height(height), Width(width) {};
		operator Coords() const { return Coords(Width, Height); };
	};
	const Bounds DefBounds = Bounds(640, 410);
	//Base Window Interface.
	class IWindow {
		Events::EventSystem OwnerEvent = *Events::GlobalEventSystem::GetInstance();
	protected:
		//Replacement for Update and Resize Functions.
		Events::EventSystem GetEventSystem() { return OwnerEvent; };
	public:
		//Should Probably be overriden.
		virtual ~IWindow() = default;
		//May be removed as all this does is Dispatch OnWindowUpdate.
		void Update()
		{
			OwnerEvent.Dispatch<Events::OnWindowUpdate>(Events::OnWindowUpdate(this));
		};
		//May be removed as all this does is Dispatch OnWindowResize.
		void Resize(int width, int height) {
			OwnerEvent.Dispatch<Events::OnWindowResize>(Events::OnWindowResize(this, width, height));
		};
		//Called On Every render update.
		virtual void OnUpdate(Events::OnWindowUpdate& e) = 0;
		//Called to Handle window resize.
		virtual void OnResize(Events::OnWindowResize& e) = 0;
		//Returns a pointer to a window native object.
		virtual void* GetNativePointer() = 0;
		//Sets V-Sync.
		virtual void SetVsync(bool vsync) = 0;
		//Return Window Bounds
		virtual Bounds GetBounds() = 0;
		virtual int GetHeight() = 0;
		virtual int GetWidth() = 0;
	protected:
		int Height, Width = 0;
		bool FullScreen = false;
		//Must call
		void Init(Application* App, Bounds WinBounds = DefBounds, std::string title = "Sphynx Engine", bool fullscreen = false)
		{
			FullScreen = fullscreen;
			Height = WinBounds.Height;
			Width = WinBounds.Width;
			OwnerEvent = App->GetAppEventSystem();
			OwnerEvent.Subscribe<IWindow, Events::OnWindowUpdate>(this, &IWindow::OnUpdate);
			OwnerEvent.Subscribe<IWindow, Events::OnWindowResize>(this, &IWindow::OnResize);
		};
	};
};