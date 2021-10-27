#pragma once
#ifndef SphynxApplication
#define SphynxApplication
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

<<<<<<< Updated upstream
<<<<<<< Updated upstream
#define Sphynx_Version "0.2.8"
=======
=======
>>>>>>> Stashed changes
#define Sphynx_Version "0.3.6"
>>>>>>> Stashed changes

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
	class Application
	{
	private:
		Core::Imgui imgui;
		//TODO:Finish the class.
		Core::ThreadPool threadpool = Core::ThreadPool();
		Events::EventSystem eventSystem;
		Core::Scripting::ScriptingEngine scriptingEngine;
		std::list<Pointer<Events::EventSystem>> EventSystemArray;
		Core::IWindow* MainWindow = nullptr;
		bool AppAlive = true;
		inline void HandleWindowClose(Events::OnWindowClose& e);
#if defined(DEBUG)
		inline void StdLog(OnLog& e) { std::cout << e.msg; };
#endif
	public:
		Application();
		static Application* GetApplication();
		virtual ~Application();
		virtual void Update() = 0;
		virtual void Start() = 0;
		void Run();
		bool HasWindow()const noexcept{ return static_cast<bool>(MainWindow); };
		inline void CloseApplication() noexcept { AppAlive = false; };
		//Should the app be aware of the eventsystems or not ?(Currently it is)
		Events::EventSystem RequestNewEventSystem();
		void DeleteEventSystem(Events::EventSystem& e);
		template<typename EventType>
		inline void DispatchToActiveEventSystems(EventType& e, bool Immediate)
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
		inline Core::Scripting::ScriptingEngine GetScriptingEngine()noexcept { return scriptingEngine; };
		//////////////Window Handling///////////////

		//Create the Main Window using a premade IWindow Pointer to allow Backend Choice(GL or DX)
		//TODO: LOL, NO more Choice.
		Core::IWindow* CreateMainWindow(std::unique_ptr<Core::IWindow>&& window /*We Want to steal it, NEW: We Don't Need it.*/);
	};
	//To be defined in a client
	Application* CreateApplication();
}
#define GetApplication() ::Sphynx::Application::GetApplication()
#endif