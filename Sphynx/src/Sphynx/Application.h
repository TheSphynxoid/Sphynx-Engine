#pragma once
#ifndef SphynxApplication
#define SphynxApplication
#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "SpTime.h"
#include "Core/Graphics/Imgui.h"
#include "Pointer.h"
#include "Logger.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <iostream>
//#define Sphynx_Version "V0.7.4-PreAlpha"

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
	/*With the current design of the engine modules and structure the first run of the game will be very slow as it'll bake ,textures, font atlases,Scripts*/
	class Application
	{
	private:
		int argc;
		char** argv;
		//TODO:Finish the class.
		Events::EventSystem eventSystem;
		Core::IWindow* MainWindow = nullptr;
		bool AppAlive = true;
#if defined(DEBUG)
		inline void StdLog(Sphynx::OnLog& e) { 
			std::cout << e.msg;
		};
		inline void StdFlush(Sphynx::OnLogFlush& e) {
			std::cout.flush();
		}
#endif
	public:
		Application();
		Application(Application&) = delete;
		Application(Application&&) = default;
		virtual ~Application();
		static Application* GetApplication();
		int GetArgs(const char** _argv) { _argv = const_cast<const char**>(argv); return argc; };
		virtual void Update() = 0;
		virtual void Start() = 0;
		void Run(int argc = 0, char** argv = nullptr);
		bool HasWindow()const noexcept{ return static_cast<bool>(MainWindow); };
		inline void CloseApplication() noexcept { AppAlive = false; };
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
		//Returns both argv and argc
		char** Getargsv(int* Argc) { Argc = &argc; return argv; };
		//////////////Window Handling///////////////

		//Create the Main Window using a premade IWindow Pointer to allow Backend Choice(GL or DX)
		//TODO: LOL, NO more Choice.
		Core::IWindow* CreateMainWindow(Core::IWindow* window);
	};
	//To be defined in a client
	Application* CreateApplication();
}
#define GetApplication() ::Sphynx::Application::GetApplication()
#define GetMainWindow() GetApplication()->GetMainWindow()
#endif