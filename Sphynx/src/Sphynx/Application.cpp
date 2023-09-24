#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "SpTime.h"
#include "Pointer.h"
#include "Core/Graphics/Window.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "glm/gtx/quaternion.hpp"
#include "Scene.h"
#include "Core/SceneManager.h"
#include "Core/Threadpool.h"
#include "Core/Scenic/Scenic.h"
#include "Core/Scripting/Mono/MonoRuntime.h"
#undef GetApplication
#undef GetMainWindow

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Core::Graphics;

Application* MainApplication;

//void Sphynx::Application::HandleWindowClose(Events::OnWindowClose& e)
//{
//	e.GetWindow()->Close();
//}

Sphynx::Application::Application()
{
	ThreadPool::Start();
	eventSystem = Events::EventSystem();
	//eventSystem.Subscribe<Application, Events::OnWindowClose>(this, &Application::HandleWindowClose);
#if defined(DEBUG)
	Events::GlobalEventSystem::GetInstance()->Subscribe<Application, OnLog>(this, &Application::StdLog);
	Events::GlobalEventSystem::GetInstance()->Subscribe<Application, OnLogFlush>(this, &Application::StdFlush);
#endif
	MainApplication = this;
	this->CreateMainWindow(IWindow::Create(this));
	//Scripting::ScriptingEngine::InitScripting();
}

Application* Sphynx::Application::GetApplication()
{
	return MainApplication;
}

Sphynx::Application::~Application()
{
	this->eventSystem.QueueEvent<Events::OnApplicationClose>(Events::OnApplicationClose());
}

void Sphynx::Application::Run(int argc, char** argv)
{
	this->argc = argc;
	this->argv = argv;
	Events::GlobalEventSystem::GetInstance()->DispatchImmediate<Events::OnApplicationStart>(Events::OnApplicationStart());
	//Scenic::WriteScene(&SceneManager::GetScene())
	Input::Init();
	SceneManager::Start();
	Sphynx::Mono::MonoRuntime Mono ("GameAssembly.dll");
	Mono.Start();
	Start();
	Time::Start();
	int i = 0;
	while (MainWindow->IsAlive()) {
		Update();
		Mono.Update();
		Events::GlobalEventSystem::GetInstance()->DispatchImmediate<Events::OnApplicationUpdate>(Events::OnApplicationUpdate());
		//Events
		Events::GlobalEventSystem::GetInstance()->Dispatch();
		eventSystem.Dispatch();
		//Physics
		// Later
		//Render
		MainWindow->GetRenderer()->Clear();
		SceneManager::Update();
		//Imgui.
		Imgui::OnOverlayUpdate();
		//Buffer Swap
		MainWindow->Update();

		Time::Update();
	}
	//SceneManager::End();
	ThreadPool::Stop();
}

Sphynx::Core::IWindow* Sphynx::Application::CreateMainWindow(Core::IWindow* window)
{
	if (!MainWindow) {
		MainWindow = window;
		Imgui::Start();
		MainWindow->Start();
	}
	else {
		Core_Warn("A Window Is Already Open.");
	}
	Events::GlobalEventSystem::GetInstance()->DispatchImmediate(Events::OnWindowOpen(MainWindow));
	return MainWindow;
}