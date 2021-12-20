#pragma once
#include "Core/Module.h"
#include "Events/ImGuiEvents.h"
#include "Core/Scripting/ScriptingEngine.h"
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
		virtual ~IOverlayWindow(){};
		virtual void Draw() = 0;
		//This Works because we expect the window to be instanced with new.
		//TODO: Change the Imgui Implementation to not need an instance of IOverlayWindow but to make it itself
		void Close() { delete this; };
		//Never Call this in the constructor as it's still null And will probably have no use of it.
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
	//TODO: Change the Imgui Implementation to not need an instance of IOverlayWindow but to make it itself
	//as this can lead to the user providing the address of a local object(example : imgui.AddOverlayWindow(&AboutWindow())).
	class Imgui final : public Module {
	private:
		Application* App;
		IWindow* window = NULL;
		std::list<IOverlayWindow*> Overlays;
		//Storing Names of the window classes for The Debug Window.
		std::list<const char*> Names;
		void ImGuiOnWindowShutdown(Events::OnWindowClose& e);
	public:
		void Start(Application* app)override;
		void OnOverlayUpdate();
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
		Scripting::ScriptingEngine scripts;
		Core::IWindow* window;
		Core::IWindow* extra;
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
