#include <Sphynx.h>
#include <Sphynx/Events/Event.h>
#include <Sphynx/Events/ApplicationEvents.h>
#include <iostream>
#include <string>
#include <Sphynx/Core/Graphics/Platform/GLWindow.h>
#include <Sphynx/Core/Platform/GLFWInput.h>

using namespace Sphynx;



class SandBox : public Sphynx::Application
{
public:
	SandBox() {
		OpenMainWindow(std::make_unique<Core::GLWindow>(this, Sphynx::Core::Bounds(1024, 576), "Legacy", NULL));
	}
	void Update() {

	}
	~SandBox() {

	}
private:

};

void OnUpdateTester(Events::OnApplicationUpdate& e) {
	static bool t = true;
	if (t) {
		Core_Info("Called");
		t = false;
	}
}

void OnWindowFocusTester(Events::OnWindowFocus& e) {
	std::cout << e.GetWindow();
}

void OnKeyPress(Sphynx::MouseButton button, Sphynx::Action action) {
	switch (action)
	{
	case Sphynx::Action::Pressed:
		if (button == MouseButton::LeftButton)Client_Trace("left Button Pressed");
		break;
	case Sphynx::Action::Repeat:
		break;
	case Sphynx::Action::Release:
		break;
	default:
		break;
	}
}

//The Main Entry Point for clients
Sphynx::Application* Sphynx::CreateApplication() {
	auto sandbox = new SandBox();
	sandbox->GetAppEventSystem()->Subscribe<Events::OnApplicationUpdate>(&OnUpdateTester);
	sandbox->GetAppEventSystem()->Subscribe<Events::OnWindowFocus>(&OnWindowFocusTester);
	auto inp = (Sphynx::Core::GLFWInput*)sandbox->GetMainWindow()->GetInput();
	inp->RegisterMouseButtonCallback<void>(Sphynx::Delegate<void,void,Sphynx::MouseButton,Action>(&OnKeyPress));
	return sandbox;
}