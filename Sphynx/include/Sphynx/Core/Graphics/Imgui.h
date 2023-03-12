#pragma once
#include "Core/Module.h"
#include "Events/ImGuiEvents.h"
#include "Core/Scripting/ScriptingEngine.h"
#include "SpTime.h"
#include <list>
#include <typeinfo>
#include <vector>
#ifdef EXPOSE_IMGUI_H
#include "../dep/imgui/imgui.h"
#endif // EXPOSE_IMGUI_H

namespace Sphynx::Core {
	class IWindow;
	class Imgui;
	//Base Imgui Windows
	class IOverlayWindow {
	private:
		Imgui* imgui;
	protected:
		bool IsOpen = true;
	public:
		virtual ~IOverlayWindow() = default;
		virtual void Draw() = 0;
		void Open() { IsOpen = true; };
		void Close() { IsOpen = false; };
		//This Works because we expect the window to be instanced with new.
		//TODO: Change the Imgui Implementation to not need an instance of IOverlayWindow but to make it itself
		void Delete() { delete this; };

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
	class Imgui final {
	private:
		static inline Application* App;
		static inline IWindow* window = NULL;
		static inline std::list<IOverlayWindow*> Overlays;
		//Storing Names of the window classes for The Debug Window.
		static inline std::list<const char*> Names;
		static void ImGuiOnWindowShutdown(Events::OnWindowClose& e);
	public:
		static void Start();
		static void OnOverlayUpdate();
		static void Shutdown();
		//Return The Number of Windows
		static int GetNumberOfWindows();
		//Get A list of type_info.
		static std::list<const char*> GetTypeInfoList();
		//Adds a window to the draw list.WIP
		//TODO: Change this so that it Creates the object instead of getting it.
		static void AddOverlayWindow(IOverlayWindow* Window);
		//Remove the window from the list.
		static void RemoveOverlayWindow(IOverlayWindow* Window);
		//Gets the Specified OverlayWindow (WIP)
		template<typename T>
		static T* GetOverlayWindow() {
			auto tid = typeid(T).name();
			for (auto w : Overlays) {
				if (typeid(*w).name() == tid) {
					return (T*)w;
				}
			}
		}
	};
	//Main Application Debug Window
	//For Some Reason the first log is always ignored(fixed)
	class DebugWindow : public Core::IOverlayWindow {
		bool WindowButton = false, TimeButton = false;
		Application* App;
		Events::EventSystem* eventsystem;
		Scripting::ScriptingEngine scripts;
		Core::IWindow* window;
		char TitleBuffer[128];
	public:
		DebugWindow();
		void Draw()override;
	};
}
