#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
<<<<<<< HEAD
#include "Core/Graphics/Platform/GLWindow.h"
#include "SpTime.h"
#include "Pointer.h"
=======
#include "SpTime.h"
#include "Pointer.h"
#include "Core/Graphics/Window.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "Core/Graphics/Platform/GLMesh.h"
#include "Core/Graphics/Platform/GLMaterial.h"
#include "Core/Graphics/Platform/GLShader.h"
>>>>>>> Dev_ComponentSystem

using namespace Sphynx;
using namespace Sphynx::Core;

<<<<<<< HEAD
void Sphynx::Application::HandleWindowClose(Events::OnWindowClose& e)
{
	if (e.GetWindow() == MainWindow) {
		CloseMainWindow();
	}
	else {
		RemoveExtraWindow(e.GetWindow());
	}
=======
Application* MainApplication;

void Sphynx::Application::HandleWindowClose(Events::OnWindowClose& e)
{
	e.GetWindow()->Close();
>>>>>>> Dev_ComponentSystem
}

Vec3 p = { 0,0,0 };
Vec4 r = { 0,0,0,0 };
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
float vertices2[] = {
	-1.5f, -1.5f, 1.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
Graphics::Mesh* g;
Graphics::Mesh* g2;
Graphics::GL::GLMaterial g2mat;
Sphynx::Application::Application() : imgui(Imgui())
{
	eventSystem = Events::EventSystem();
	eventSystem.Subscribe<Application, Events::OnWindowClose>(this, &Application::HandleWindowClose);
#if defined(DEBUG)
	Events::GlobalEventSystem::GetInstance()->Subscribe<Application, OnLog>(this, &Application::StdLog);
#endif
<<<<<<< HEAD
=======
	scriptingEngine = Core::Scripting::ScriptingEngine();
	scriptingEngine.Start(this);
	scriptingEngine.GetLua().ExecuteFile("Test.lua");
>>>>>>> Dev_ComponentSystem
	if (MainWindow) {
		imgui.Start(this);
		imgui.AddOverlayWindow(new DebugWindow(this));
		//imgui.AddOverlayWindow(new AboutWindow());
		//imgui.AddOverlayWindow(new DemoWindow());
		MainWindow->Start();
	}
<<<<<<< HEAD
=======
	MainApplication = this;
}

Application* Sphynx::Application::GetApplication()
{
	return MainApplication;
>>>>>>> Dev_ComponentSystem
}

Sphynx::Application::~Application()
{
	this->eventSystem.QueueEvent<Events::OnApplicationClose>(Events::OnApplicationClose());
}

void Sphynx::Application::Run()
{
	eventSystem.DispatchImmediate<Events::OnApplicationStart>(Events::OnApplicationStart());
<<<<<<< HEAD
	while (AppAlive) {
		Update();
		eventSystem.DispatchImmediate<Events::OnApplicationUpdate>(Events::OnApplicationUpdate());
		time.Update();
		if (MainWindow != NULL) {
			if (MainWindow->IsAlive()) {
				MainWindow->Update();
				for (auto& win : ExtraWindows) {
					win->Update();
				}
			}
		}
		//Events
		Events::GlobalEventSystem::GetInstance()->Dispatch();
		eventSystem.Dispatch();
		for (auto& es : EventArray) {
			es->Dispatch();
		}
	}
	imgui.Shutdown();
	for (auto& win : ExtraWindows) {
		//A Closed Window doesn't exist.
		RemoveExtraWindow(win);
	}
	GLWindow::TerminateGLFW();
	time.Shutdown();
}

void DTest(int i) {
	std::cout << "wut " << i;
}

Events::EventSystem Sphynx::Application::RequestNewEventSystem()
{
	Pointer<Events::EventSystem>* e = AllocatePointer<Events::EventSystem>();
	EventArray.push_back(*e);
	e->SetDestroyCallBack<Application>(Delegate<void, Application, Events::EventSystem&>(this, &Application::DeleteEventSystem));
	return *e->GetRaw();
}

void Sphynx::Application::DeleteEventSystem(Events::EventSystem& e)
{
	for (auto& ptr : EventArray) {
		if (ptr.GetRaw() == &e) {
			//EventArray.remove(ptr);
			EventArray.remove_if([&,ptr](Pointer<Events::EventSystem>& p)->bool {return ptr.GetAddress() == p.GetAddress(); });
			ptr.Release();
			delete &ptr;
		}
	}
}

Time* Sphynx::Application::GetTimeObject()
=======
	threadpool.Start(this);
	//threadpool.Submit(Delegate<void,void>(&test));
	//threadpool.Submit(Delegate<void, void>(&test2));
	Time::Start();
	g = new Graphics::GL::GLMesh();
	g->Create(vertices, sizeof(vertices), nullptr, 0, Core::Graphics::MeshType::Static);
	g2 = new Graphics::GL::GLMesh();
	g2->Create(vertices2, sizeof(vertices2), nullptr, 0, Core::Graphics::MeshType::Static);
	auto s = Graphics::GL::GLShader();
	auto s1 = Graphics::GL::GLShader();
	s.CreateFromCode(DEF_FSHADER, Graphics::ShaderType::FragmentShader);
	s1.CreateFromCode("#version 330 core\n"
		"layout(location = 0) in vec3 aPos;"
		"out vec4 vertexColor;"
		"void main()"
		"{"
		"	gl_Position = vec4(aPos, 1.0);"
		"	vertexColor = vec4(1.0, 0.0, 0.0, 0.5);"
		" }", Graphics::ShaderType::VertexShader);
	g2mat = Graphics::GL::GLMaterial(Graphics::ShaderPack(&s1, &s, nullptr, nullptr), nullptr);
	while (AppAlive) {
		Update();
		eventSystem.DispatchImmediate<Events::OnApplicationUpdate>(Events::OnApplicationUpdate());
		//Events
		Events::GlobalEventSystem::GetInstance()->Dispatch();
		eventSystem.Dispatch();
		for (auto& es : EventSystemArray) {
			es->Dispatch();
		}
		UpdateWindow();
		Time::Update();
	}
	threadpool.Stop();
	imgui.Shutdown();
	delete g;
	delete g2;
}

void Sphynx::Application::UpdateWindow()
{
	if (MainWindow != NULL) {
		if (MainWindow->IsAlive()) {
			MainWindow->GetRenderer()->Submit(Core::Graphics::RenderObject(g, nullptr, p, r));
			MainWindow->GetRenderer()->Submit(Core::Graphics::RenderObject(g, &g2mat, p, r));
			MainWindow->Update();
		}
	}
}

Events::EventSystem Sphynx::Application::RequestNewEventSystem()
{
	Pointer<Events::EventSystem>* e = AllocatePointer<Events::EventSystem>();
	EventSystemArray.push_back(*e);
	e->SetDestroyCallBack<Application>(Delegate<void, Application, Events::EventSystem&>(this, &Application::DeleteEventSystem));
	return *e->GetRaw();
}

void Sphynx::Application::DeleteEventSystem(Events::EventSystem& e)
>>>>>>> Dev_ComponentSystem
{
	for (auto& ptr : EventSystemArray) {
		if (ptr.GetRaw() == &e) {
			EventSystemArray.remove_if([&,ptr](Pointer<Events::EventSystem>& p)->bool {return ptr.GetRaw() == p.GetRaw(); });
			ptr.Release();
			delete &ptr;
		}
	}
}

<<<<<<< HEAD
void Sphynx::Application::OpenMainWindow(std::unique_ptr<Core::IWindow>&& window)
{
	if (!MainWindow) {
		//Stolen!
=======
Sphynx::Core::IWindow* Sphynx::Application::CreateMainWindow(std::unique_ptr<Core::IWindow>&& window)
{
	if (!MainWindow) {
		//Pointer Stolen!
>>>>>>> Dev_ComponentSystem
		Core::IWindow* obj = window.release();
		MainWindow = obj;
		imgui.Start(this);
		imgui.AddOverlayWindow(new DebugWindow(this));
<<<<<<< HEAD
		//imgui.AddOverlayWindow(new AboutWindow());
		//imgui.AddOverlayWindow(new DemoWindow());
		MainWindow->Start();
	}
	else {
		Core_Warn("Non Fatal : Overwriting Main Window is not allowed. Close it and then Open a new one.");
	}
}

void Sphynx::Application::CloseMainWindow()
{
	imgui.Shutdown();
	//HELL.
	auto placeholder = MainWindow;
	MainWindow = NULL;
	delete placeholder;
	//MainWindow->~IWindow();
}

void Sphynx::Application::CloseAllWindow()
{
	CloseMainWindow();
	for (auto& win : ExtraWindows) {
		//A Closed Window doesn't exist.
		//Issue: Closing a extra window Raises an event that Call RemoveExtraWindow with the same window.
		//I Don't know if it will cause problems.
		RemoveExtraWindow(win);
	}
}

void Sphynx::Application::AddExtraWindow(std::unique_ptr<Core::IWindow>&& window)
{
	//Ours now!
	Core::IWindow* obj = window.release();
	obj->Start();
	ExtraWindows.push_back(obj);
}

void Sphynx::Application::RemoveExtraWindow(Core::IWindow* window)
{
	ExtraWindows.remove(window);
}
=======
		MainWindow->Start();
	}
	else {
		Core_Warn("A Window Is Already Open.");
	}
	return MainWindow;
}
>>>>>>> Dev_ComponentSystem
