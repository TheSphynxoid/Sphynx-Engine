#pragma once
#include <sstream>

#include "Events/WindowEvents.h"
#include "Application.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "Core.h"
#include "Input.h"

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
	//Base Window Interface.(Each Derived class must ensure Input, Closing...)
	class IWindow {
	private:
		Events::EventSystem OwnerEvent = *Events::GlobalEventSystem::GetInstance();
		//Ensure that this instance is Initialized.
		bool InstanceHasInit = false;
	protected:
		Input* input;
		//Replacement for Update and Resize Functions.
		//Current State of the window.

		virtual void Internal_ChangeTitle(const char* title) = 0;
	public:
		virtual inline bool IsAlive() = 0;
		//Should Probably be overriden.
		virtual ~IWindow() = default;
		//Start Window.
		void Start() {
			if (!InstanceHasInit)
				throw "Not Initialized. Any Class That Derives from IWindow Must Call Init";
		}
		//May be removed as all this does is Queue OnWindowUpdate.
		void Update()
		{
			OnUpdate();
			OwnerEvent.QueueEvent<Events::OnWindowUpdate>(Events::OnWindowUpdate(this));
		};
		void Close() {
			OwnerEvent.QueueEvent<Events::OnWindowClose>(Events::OnWindowClose(this));
		}
		void Resize(int width, int height) {
			if (!InstanceHasInit)
				throw "Not Initialized. Any Class That Derives from IWindow Must Call Init";
			this->Width = width;
			this->Height = height;
			OwnerEvent.QueueEvent<Events::OnWindowResize>(Events::OnWindowResize(this, width, height));
		};

		virtual void OnClose() = 0;
		//Called On Every render update.
		virtual void OnUpdate() = 0;
		//Called to Handle window resize.
		virtual void OnResize(Events::OnWindowResize& e) = 0;
		//Returns a pointer to a window native object.
		virtual void* GetNativePointer() = 0;
		//Returns a bool true if vsync is enabled
		virtual bool IsVsyncEnabled() = 0;
		//Sets V-Sync.
		virtual void SetVsync(bool vsync) = 0;
		//Return Window Bounds
		virtual Bounds GetBounds() = 0;
		virtual int GetHeight() = 0;
		virtual int GetWidth() = 0;
		const char* GetTitle() {
			return Title.c_str();
		};

		//Request Title Change
		void ChangeTitle(const char* title) {
			Title = title;
			Internal_ChangeTitle(title);
		};

		///////////Graphics//////////////
		virtual Sphynx::Core::Graphics::IRenderer* GetRenderer() = 0;

		//Returns the Window's EventSystem.
		Events::EventSystem* GetEventSystem() { return &OwnerEvent; };
		virtual Input* GetInput() { return this->input; };
	protected:
		int Height = 0, Width = 0;
		bool FullScreen = false;
		std::string Title;
		//Initializes the instance. Must Be call by any class that derives from IWindow.
		void Init(Application* App, Bounds WinBounds = DefBounds, std::string title = "Sphynx Engine", bool fullscreen = false)
		{
			FullScreen = fullscreen;
			Height = WinBounds.Height;
			Width = WinBounds.Width;
			Title = title;
			OwnerEvent = App->RequestNewEventSystem();
			//input = Input();
			
			//App->GetAppEventSystem()->Subscribe<IWindow, Events::OnWindowResize>(this, &IWindow::OnResize);
			InstanceHasInit = true;
		};
	};
};