#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "SpTime.h"
#include "Pointer.h"
#include "Core/Graphics/Window.h"
#include "Core/Graphics/Pipeline/Renderer.h"
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
#include "Core/Graphics/Platform/GLMesh.h"
#include "Core/Graphics/Platform/GLMaterial.h"
#include "Core/Graphics/Platform/GLShader.h"
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
#include "glm/gtx/quaternion.hpp"
#include "GameObject.h"
#include "Camera.h"
#include "Core/MeshRenderer.h"
>>>>>>> Stashed changes
#undef GetApplication
using namespace Sphynx;
using namespace Sphynx::Core;

Application* MainApplication;

//void Sphynx::Application::HandleWindowClose(Events::OnWindowClose& e)
//{
//	e.GetWindow()->Close();
//}

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
float texCoords[] = {
	0.0f, 0.0f,  // lower-left corner  
	1.0f, 0.0f,  // lower-right corner
	0.5f, 1.0f   // top-center corner
};
Graphics::Mesh* g;
Graphics::Mesh* g2;
Graphics::GL::GLMaterial g2mat;
Sphynx::Application::Application() : imgui(Imgui())
{
	eventSystem = Events::EventSystem();
	//eventSystem.Subscribe<Application, Events::OnWindowClose>(this, &Application::HandleWindowClose);
#if defined(DEBUG)
	Events::GlobalEventSystem::GetInstance()->Subscribe<Application, OnLog>(this, &Application::StdLog);
#endif
	scriptingEngine = Core::Scripting::ScriptingEngine();
	scriptingEngine.Start(this);
	scriptingEngine.GetLua().ExecuteFile("Test.lua");
	MainApplication = this;
}

Application* Sphynx::Application::GetApplication()
{
	return MainApplication;
}

Sphynx::Application::~Application()
{
	this->eventSystem.QueueEvent<Events::OnApplicationClose>(Events::OnApplicationClose());
}

class MovementComp : public Sphynx::Component {
	// Inherited via Component
	virtual void OnComponentAttach(GameObject* parent) override
	{
		Core_Info("MovementComp Attached to {0}",parent->GetID());
	}
	virtual void OnComponentDetach() override
	{
	}
	virtual void Update() override
	{
		if (Input::IsKeyPressed(Keys::Up))
			GetTransform()->Translate(glm::vec3(0.0f, 0.5f * Time::GetDeltaTime(), 0.0f));
		if (Input::IsKeyPressed(Keys::Down))
			GetTransform()->Translate(glm::vec3(0.0f, -0.5f * Time::GetDeltaTime(), 0.0f));
		if (Input::IsKeyPressed(Keys::Left))
			GetTransform()->Rotate(60 * Time::GetDeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		if (Input::IsKeyPressed(Keys::Right))
			GetTransform()->Rotate(-60 * Time::GetDeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
};

void Sphynx::Application::Run()
{
	Camera::PrimaryCamera->AddComponent<Camera>();
	eventSystem.DispatchImmediate<Events::OnApplicationStart>(Events::OnApplicationStart());
	threadpool.Start(this);
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
	//threadpool.Submit(Delegate<void,void>(&test));
	//threadpool.Submit(Delegate<void, void>(&test2));
	Time::Start();
	Input::Init();
	g = Graphics::Mesh::Create(vertices, sizeof(vertices), nullptr, 0, Core::Graphics::MeshType::Static);
	g2 = Graphics::Mesh::Create(vertices2, sizeof(vertices2), nullptr, 0, Core::Graphics::MeshType::Static);
	auto s = Graphics::GL::GLShader();
	auto s1 = Graphics::GL::GLShader();
	s.CreateFromCode(DEF_FSHADER, Graphics::ShaderType::FragmentShader);
	s1.CreateFromCode("#version 330 core\n"
		"layout(location = 0) in vec3 aPos;"
		"out vec4 vertexColor;"
		"void main()"
		"{"
		"	gl_Position = vec4(aPos, 1.0);"
		"	vertexColor = vec4(0.0, 1.0, 0.0, 0.5);"
		" }", Graphics::ShaderType::VertexShader);
	g2mat = Graphics::GL::GLMaterial(Graphics::ShaderPack(&s1, &s, nullptr, nullptr), (Graphics::GL::GLTexture*)nullptr);
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
	Camera::PrimaryCamera->GetTransform()->Translate(glm::vec3(0.0f, 0.0f, -3.0f));
	auto Square = GameObject::CreatePrimitive(Primitives::Cube);
	auto Square2 = GameObject::CreatePrimitive(Primitives::Cube);
	Square2.GetTransform()->Translate(glm::vec3(3.0f, 0.0f, 0.0f));
	Square.GetTransform()->Translate(glm::vec3(-3.0f, 0.0f, 0.0f));
	Square.GetTransform()->Rotate(30, glm::vec3(0.0f, 1.0f, 0.0f));
	Camera::PrimaryCamera->AddComponent<MovementComp>();
	Input::Init();
	Start();
	Time::Start();
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
	while (AppAlive) {
		Update();
		eventSystem.DispatchImmediate<Events::OnApplicationUpdate>(Events::OnApplicationUpdate());
		//Events
		Events::GlobalEventSystem::GetInstance()->Dispatch();
		eventSystem.Dispatch();
		for (auto& es : EventSystemArray) {
			es->Dispatch();
		}
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
		UpdateWindow();
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
		Square.Update();
		Square2.Update();
		Camera::PrimaryCamera->Update();
		if (MainWindow != NULL) {
			if (MainWindow->IsAlive()) {
				MainWindow->Update();
			}
		}
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
		Time::Update();
	}
	imgui.Shutdown();
<<<<<<< Updated upstream
	delete g;
	delete g2;
}

void Sphynx::Application::UpdateWindow()
{
	if (MainWindow != NULL) {
		if (MainWindow->IsAlive()) {
			MainWindow->GetRenderer()->Submit(Core::Graphics::RenderObject(g2, &g2mat, p, r));
			MainWindow->GetRenderer()->Submit(Core::Graphics::RenderObject(g, nullptr, p, r));
			MainWindow->Update();
		}
	}
=======
	threadpool.Stop();
>>>>>>> Stashed changes
=======
		Time::Update();
	}
	imgui.Shutdown();
	threadpool.Stop();
>>>>>>> Stashed changes
=======
		Time::Update();
	}
	imgui.Shutdown();
	threadpool.Stop();
>>>>>>> Stashed changes
}

Events::EventSystem Sphynx::Application::RequestNewEventSystem()
{
	Pointer<Events::EventSystem>* e = AllocatePointer<Events::EventSystem>();
	EventSystemArray.push_back(*e);
	e->SetDestroyCallBack<Application>(Delegate<void, Application, Events::EventSystem&>(this, &Application::DeleteEventSystem));
	return *e->GetRaw();
}

void Sphynx::Application::DeleteEventSystem(Events::EventSystem& e)
{
	for (auto& ptr : EventSystemArray) {
		if (ptr.GetRaw() == &e) {
			EventSystemArray.remove_if([&,ptr](Pointer<Events::EventSystem>& p)->bool {return ptr.GetRaw() == p.GetRaw(); });
			ptr.Release();
			delete &ptr;
		}
	}
}

Sphynx::Core::IWindow* Sphynx::Application::CreateMainWindow(std::unique_ptr<Core::IWindow>&& window)
{
	if (!MainWindow) {
		//Pointer Stolen!
		Core::IWindow* obj = window.release();
		MainWindow = obj;
		imgui.Start(this);
		imgui.AddOverlayWindow(new DebugWindow(this));
		MainWindow->Start();
	}
	else {
		Core_Warn("A Window Is Already Open.");
	}
	eventSystem.DispatchImmediate(Events::OnWindowOpen(MainWindow));
	return MainWindow;
}