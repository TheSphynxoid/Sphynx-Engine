#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "SpTime.h"
#include "Core/Graphics/Imgui.h"
#include "Pointer.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <iostream>

//Imgui.Window.EventSystem.GLFW.Broken Time Class
#define Sphynx_Version "0.05.5 WIP"

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
	class Application
	{
	protected:
		Core::Imgui imgui;
		//Window API Provider.
	private:
		Events::EventSystem eventSystem;
		std::list<Pointer<Events::EventSystem>> EventArray;
		Core::IWindow* MainWindow = NULL;
		std::list<Core::IWindow*> ExtraWindows;
		Sphynx::Time time;
		bool AppAlive = true;
		void HandleWindowClose(Events::OnWindowClose& e);
#if defined(DEBUG)
		void StdLog(OnLog& e) { std::cout << e.msg; };
#endif
	public:
		Application();
		virtual ~Application();
		virtual void Update() = 0;
		void Run();
		void CloseApplication() { AppAlive = false; };
		Events::EventSystem* GetAppEventSystem()noexcept { return &eventSystem; };
		//Should the app be aware of the eventsystems or not ?(Currently it is)
		Events::EventSystem RequestNewEventSystem();
		void DeleteEventSystem(Events::EventSystem& e);
		template<typename EventType>
		void DispatchToActiveEventSystems(EventType& e,bool Immediate) 
		{
			for (auto& es : EventArray) {
				if (Immediate) {
					es->DispatchImmediate(e);
				}
				else es->QueueEvent(e);
			}
		}
		Core::IWindow* GetMainWindow()noexcept { return MainWindow; };
		Sphynx::Time* GetTimeObject();

		//////////////Window Handling///////////////
		
		void OpenMainWindow(std::unique_ptr<Core::IWindow>&& window /*We Want to steal it*/);
		void CloseMainWindow();
		void CloseAllWindow();
		void AddExtraWindow(std::unique_ptr<Core::IWindow>&& window);
		//AKA. Close The Extra Window
		void RemoveExtraWindow(Core::IWindow* window);
		//Returns a reference of the vector.
		inline std::list<Core::IWindow*> GetExtraWindows() {
			return ExtraWindows;
		}
		inline std::list<Core::IWindow*>::const_iterator ExtraWindowsBegin() {
			return ExtraWindows.begin();
		}		
		inline std::list<Core::IWindow*>::const_iterator ExtraWindowsEnd() {
			return ExtraWindows.end();
		}
	};
	//New Name For Application.To Be Removed (it's here just to help me design the engine)
	typedef Application Game;
	//To be defined in a client
	Application* CreateApplication();
}
