#include <Sphynx.h>
#include <Sphynx/Events/ApplicationEvents.h>
#include <iostream>
#include <string>
#include <Sphynx/Core/Graphics/Platform/GLWindow.h>

using namespace Sphynx;

class SandBox : public Sphynx::Application
{
public:
	SandBox() {
		Sphynx::Core::Bounds b = { 1024, 768 };
		CreateMainWindow(std::make_unique<Core::GLWindow>(this, b, "SandBox", false));
	}
	void Update() {
		if (HasWindow()) {
			if (Sphynx::Input::IsKeyPressed(Sphynx::Keys::Left)) {
				Core_Info("Left Key Pressed");
			}if (Sphynx::Input::IsMouseButtonPressed(Sphynx::MouseButton::LeftButton)) {
				Core_Info("Left Mouse Button Pressed");
			}
		}
	}
	~SandBox() {

	}
};

//The Main Entry Point for clients
Sphynx::Application* Sphynx::CreateApplication() {
	auto sandbox = new SandBox();
	return sandbox;
}