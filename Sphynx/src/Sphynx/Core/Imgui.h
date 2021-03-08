#pragma once
#include "Module.h"
#include "Events/ImGuiEvents.h"
#include "SpTime.h"
#include <list>
#include <typeinfo>
#include <vector>

namespace Sphynx {
	struct OnLog;
}
namespace Sphynx::Core {
	class IWindow;
	class Imgui;
	//Base Imgui Windows
	class IOverlayWindow {
	private:
		Imgui* imgui;
		void InternalSetup(Imgui* instance);
	protected:
		bool IsOpen = true;
	public:
		//IOverlayWindow() = default;
		virtual void Draw() = 0;
		void Close() { delete this; };

		Imgui* GetImGui() { return imgui; };
		friend class Imgui;
	};
	class DemoWindow final : public IOverlayWindow {
	public:
		void Draw()override;
	};
	class AboutWindow final : public IOverlayWindow {
		void Draw()override;
	};
	class Imgui final : public Module {
	private:
		Application* App;
		IWindow* window = NULL;
		std::list<IOverlayWindow*> Overlays;
		Time* time;
		//Storing Names of the window classes for The Debug Window.
		std::list<const char*> Names;
	public:
		void Start(Application* app)override;
		void OnOverlayUpdate(Events::OnOverlayUpdate& e);
		void Update()override;
		void Shutdown()override;
		//We Can Also Use the event System. Instead of holding window we just dispatch an event.

		//Return The Number of Windows
		int GetNumberOfWindows();
		//Get A list of type_info.
		std::list<const char*> GetTypeInfoList();
		//Adds a window to the draw list.WIP
		void AddOverlayWindow(IOverlayWindow* Window);
		//Remove the window from the list.
		void RemoveOverlayWindow(IOverlayWindow* Window);
	};
	//Main Application Debug Window
	//For Some Reason the first log is always ignored(fixed)
	class DebugWindow : public Core::IOverlayWindow {
		bool WindowButton = false, TimeButton = false;
		Application* App;
		Events::EventSystem* eventsystem;
		Core::IWindow* window;
		char TitleBuffer[128];
		//Put the in the cpp. I Couldn't include imgui.h because of SandBox.
		//std::vector<char> Buf;
		//std::vector<int> LineOffsets;

		void OnEventLog(OnLog& e);
	public:
		DebugWindow(Application* app);
		void Draw()override;

	};
}
