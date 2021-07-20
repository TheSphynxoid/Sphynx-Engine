#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "SpTime.h"
#include "Core/Graphics/Imgui.h"
#include "Pointer.h"
#include "Core/Scripting/ScriptingEngine.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <iostream>
#include "Core/ThreadPool.h"
//Imgui&Window&EventSystem&GLFW&Scripting&Time&GLRenderer(WIP) Classes
#define Sphynx_Version "0.2.2 WIP"

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
	class Application
	{
	private:
		//Window API Provider.
		Core::Imgui imgui;
		Core::ThreadPool threadpool = Core::ThreadPool();
		Events::EventSystem eventSystem;
		Core::Scripting::ScriptingEngine scriptingEngine;
		std::list<Pointer<Events::EventSystem>> EventSystemArray;
		Core::IWindow* MainWindow = nullptr;
		Core::IWindow* ExtraWindow = nullptr;
		bool AppAlive = true;
		inline void HandleWindowClose(Events::OnWindowClose& e);
#if defined(DEBUG)
		inline void StdLog(OnLog& e) { std::cout << e.msg; };
#endif
	public:
		Application();
		virtual ~Application();
		virtual void Update() = 0;
		void Run();
		void UpdateWindow();
		inline void CloseApplication() noexcept { AppAlive = false; };
		//Should the app be aware of the eventsystems or not ?(Currently it is)
		Events::EventSystem RequestNewEventSystem();
		void DeleteEventSystem(Events::EventSystem& e);
		template<typename EventType>
		void DispatchToActiveEventSystems(EventType& e, bool Immediate)
		{
			for (auto& es : EventSystemArray) {
				if (Immediate) {
					es->DispatchImmediate(e);
				}
				else es->QueueEvent(e);
			}
		}
		//////////////Getters///////////////////////

		inline Events::EventSystem* GetAppEventSystem()noexcept { return &eventSystem; };
		inline Core::IWindow* GetMainWindow()noexcept { return MainWindow; };
		inline Core::IWindow* GetExtraWindow()noexcept { return ExtraWindow; };
		inline Core::Scripting::ScriptingEngine GetScriptingEngine()noexcept { return scriptingEngine; };
		//////////////Window Handling///////////////

		///Create the Main Window using a premade IWindow Pointer to allow Backend Choice(GL or DX)
		Core::IWindow* CreateMainWindow(std::unique_ptr<Core::IWindow>&& window /*We Want to steal it*/);
		void CloseAllWindows();
		Core::IWindow* CreateExtraWindow(std::unique_ptr<Core::IWindow>&& window);
		//AKA. Close The Extra Window
		void CloseExtraWindow();
	};
	//To be defined in a client
	Application* CreateApplication();
}
