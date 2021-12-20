#include "pch.h"
#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "SpTime.h"
#include "Pointer.h"
#include "Core/Graphics/Window.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "glm/gtx/quaternion.hpp"
#include "GameObject.h"
#include "Camera.h"
#include "Core/MeshRenderer.h"
#include "Core/Scripting/AsScript.h"
#include "Core/SceneManager.h"
#include "Core/Graphics/Pipeline/FrameBuffer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"
#include <filesystem>
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

Sphynx::Application::Application() : imgui(Imgui())
{
	eventSystem = Events::EventSystem();
	//eventSystem.Subscribe<Application, Events::OnWindowClose>(this, &Application::HandleWindowClose);
#if defined(DEBUG)
	Events::GlobalEventSystem::GetInstance()->Subscribe<Application, OnLog>(this, &Application::StdLog);
#endif
	MainApplication = this;
	this->CreateMainWindow(IWindow::Create(this));
	Scripting::ScriptingEngine::InitScripting();
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
		Core_Info("MovementComp Attached to {0}", parent->GetID());
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
class ScreenShot : public Component {
private:
	Camera* cam;
	size_t Count;
	// Inherited via Component
	virtual void OnComponentAttach(GameObject* parent) override
	{
		cam = GetGameObject()->GetComponent<Camera>();
		stbi_flip_vertically_on_write(true);
	}
	virtual void OnComponentDetach()override {
		for (int i = 0; i < Count; ++i) {
			std::string name = "Tex";
			name += std::to_string(Count) + ".bmp";
			std::remove(name.c_str());
		}
	}

public:
	virtual void Update()override {
		if (Input::IsKeyPressed(Keys::Enter)) {
			cam->GetFrameBuffer()->Bind();
			auto tex = cam->GetFrameBuffer()->GetAttachment(1);
			auto buf = tex->ReadAllPixels(TextureDataFormat::UInt_8_8_8_8_REV);
			cam->GetFrameBuffer()->Unbind();
			std::string name = "Tex";
			name += std::to_string(Count) + ".bmp";
			stbi_write_bmp(name.c_str(), tex->GetWidth(), tex->GetHeight(), 4, buf);
			++Count;
		}
	}
};

void Sphynx::Application::Run()
{
	Events::GlobalEventSystem::GetInstance()->DispatchImmediate<Events::OnApplicationStart>(Events::OnApplicationStart());
	threadpool.Start(this);
	Input::Init();
	Start();
	SceneManager::Start();
	auto Square = GameObject::CreatePrimitive(Primitives::Cube);
	auto Square2 = GameObject::CreatePrimitive(Primitives::Plane);
	Square2.GetTransform()->Translate(glm::vec3(3.0f, 0.0f, -3.0f));
	Square.GetTransform()->Translate(glm::vec3(0.0f, 0.0f, -3.0f));
	Square.GetTransform()->Rotate(30, glm::vec3(0.0f, 1.0f, 0.0f));
	Square.AddComponent<Sphynx::Core::Scripting::AsScript>("AsScript.as", "TestModule");
	Square.AddComponent<MovementComp>();
	SceneManager::GetScene().GetPrimaryCameraObject().AddComponent<ScreenShot>();
	SceneManager::GetScene().AddGameObject(&Square);
	SceneManager::GetScene().AddGameObject(&Square2);
	Time::Start();
	int i = 0;
	while (AppAlive) {
		Update();
		Events::GlobalEventSystem::GetInstance()->DispatchImmediate<Events::OnApplicationUpdate>(Events::OnApplicationUpdate());
		//Events
		Events::GlobalEventSystem::GetInstance()->Dispatch();
		eventSystem.Dispatch();
		for (auto& es : EventSystemArray) {
			es->Dispatch();
		}
		if (MainWindow->IsAlive()) {
			SceneManager::Update();
			//FrameBuffer Render (Works)
			SceneManager::GetScene().GetPrimaryCamera()->GetFrameBuffer()->Bind();
			MainWindow->GetRenderer()->Clear();
			MainWindow->GetRenderer()->Render();
			SceneManager::GetScene().GetPrimaryCamera()->GetFrameBuffer()->Unbind();
			//Main Window Render
			SceneManager::UpdateCamera();
			MainWindow->GetRenderer()->SetDepthTesting(false);
			MainWindow->GetRenderer()->Clear();
			MainWindow->GetRenderer()->Render();
			//Imgui.
			imgui.OnOverlayUpdate();
			MainWindow->GetRenderer()->SetDepthTesting(true);
			//Buffer Swap
			MainWindow->Update();
		}
		Time::Update();
	}
	imgui.Shutdown();
	threadpool.Stop();
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
			EventSystemArray.remove_if([&, ptr](Pointer<Events::EventSystem>& p)->bool {return ptr.GetRaw() == p.GetRaw(); });
			ptr.Release();
			delete& ptr;
		}
	}
}

Sphynx::Core::IWindow* Sphynx::Application::CreateMainWindow(Core::IWindow* window)
{
	if (!MainWindow) {
		MainWindow = window;
		imgui.Start(this);
		imgui.AddOverlayWindow(new DebugWindow(this));
		MainWindow->Start();
	}
	else {
		Core_Warn("A Window Is Already Open.");
	}
	Events::GlobalEventSystem::GetInstance()->DispatchImmediate(Events::OnWindowOpen(MainWindow));
	return MainWindow;
}