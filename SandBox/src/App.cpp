#include <Sphynx.h>
#include <Sphynx/Events/ApplicationEvents.h>
#include <iostream>
#include <string>
#include <Sphynx/Core/Graphics/Platform/GLWindow.h>
#include <Sphynx/Core/Transform.h>
#include <Sphynx/Camera.h>

using namespace Sphynx;

class SandBox : public Sphynx::Application
{
private:
	GameObject* Cam = nullptr;
	Transform* TransCam = nullptr;
	Camera* CamComp = nullptr;
public:
	SandBox() {
		Sphynx::Core::Bounds b = { 512, 384 };
		this->GetMainWindow()->SetClearColor({ 0,1,0,1 });
		this->GetMainWindow()->Resize(b.Width, b.Height);
	}
	void Start() {
	}
	void Update() {
	}
	~SandBox() {

	}
};

//The Main Entry Point for clients
Sphynx::Application* Sphynx::CreateApplication() {
	auto sandbox = new SandBox();
	return sandbox;
}