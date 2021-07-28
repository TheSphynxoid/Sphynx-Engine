#pragma once
#ifndef SphynxApplication
#define SphynxApplication
#include "Core.h"
#include "Events/Event.h"
#include "SpTime.h"
#include "Core/Graphics/Imgui.h"
#include "Pointer.h"
<<<<<<< HEAD
=======
#include "Core/Scripting/ScriptingEngine.h"
>>>>>>> Dev_ComponentSystem
#include <type_traits>
#include <utility>
#include <memory>
#include <iostream>
<<<<<<< HEAD

//Imgui.Window.EventSystem.GLFW.Broken Time Class
#define Sphynx_Version "0.05.5 WIP"
=======
#include "Core/ThreadPool.h"
//Imgui&Window&EventSystem&GLFW&Scripting&Time&GLRenderer(WIP) Classes
#define Sphynx_Version "0.2.25 WIP"
>>>>>>> Dev_ComponentSystem

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
	class Application
	{
<<<<<<< HEAD
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
=======
	private:
		//Window API Provider.
		Core::Imgui imgui;
		Core::ThreadPool threadpool = Core::ThreadPool();
		Events::EventSystem eventSystem;
		Core::Scripting::ScriptingEngine scriptingEngine;
		std::list<Pointer<Events::EventSystem>> EventSystemArray;
		Core::IWindow* MainWindow = nullptr;
		bool AppAlive = true;
		inline void HandleWindowClose(Events::OnWindowClose& e);
#if defined(DEBUG)
		inline void StdLog(OnLog& e) { std::cout << e.msg; };
>>>>>>> Dev_ComponentSystem
#endif
	public:
		Application();
		static Application* GetApplication();
		virtual ~Application();
		virtual void Update() = 0;
		void Run();
<<<<<<< HEAD
		void CloseApplication() { AppAlive = false; };
		Events::EventSystem* GetAppEventSystem()noexcept { return &eventSystem; };
=======
		void UpdateWindow();
		inline void CloseApplication() noexcept { AppAlive = false; };
>>>>>>> Dev_ComponentSystem
		//Should the app be aware of the eventsystems or not ?(Currently it is)
		Events::EventSystem RequestNewEventSystem();
		void DeleteEventSystem(Events::EventSystem& e);
		template<typename EventType>
<<<<<<< HEAD
		void DispatchToActiveEventSystems(EventType& e,bool Immediate) 
		{
			for (auto& es : EventArray) {
=======
		inline void DispatchToActiveEventSystems(EventType& e, bool Immediate)
		{
			for (auto& es : EventSystemArray) {
>>>>>>> Dev_ComponentSystem
				if (Immediate) {
					es->DispatchImmediate(e);
				}
				else es->QueueEvent(e);
			}
		}
<<<<<<< HEAD
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
=======
		//////////////Getters///////////////////////

		inline Events::EventSystem* GetAppEventSystem()noexcept { return &eventSystem; };
		inline Core::IWindow* GetMainWindow()noexcept { return MainWindow; };
		inline Core::Scripting::ScriptingEngine GetScriptingEngine()noexcept { return scriptingEngine; };
		//////////////Window Handling///////////////

		///Create the Main Window using a premade IWindow Pointer to allow Backend Choice(GL or DX)
		Core::IWindow* CreateMainWindow(std::unique_ptr<Core::IWindow>&& window /*We Want to steal it*/);
>>>>>>> Dev_ComponentSystem
	};
	//New Name For Application.To Be Removed (it's here just to help me design the engine)
	typedef Application Game;
	//To be defined in a client
	Application* CreateApplication();
}
#endif