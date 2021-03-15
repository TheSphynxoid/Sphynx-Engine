#include <Sphynx.h>
#include <Sphynx/Events/Event.h>
#include <Sphynx/Events/ApplicationEvents.h>
#include <iostream>
#include <string>
#include <Sphynx/Core/Graphics/Platform/GLWindow.h>


using namespace Sphynx;



class SandBox : public Sphynx::Application
{
public:
	SandBox() {
		OpenMainWindow(std::make_unique<Core::GLWindow>(this, Bounds(1024, 576), "Legacy", false));
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

//The Main Entry Point for clients
Sphynx::Application* Sphynx::CreateApplication() {
	auto sandbox = new SandBox();
	sandbox->GetAppEventSystem()->Subscribe<Events::OnApplicationUpdate>(&OnUpdateTester);
	sandbox->GetAppEventSystem()->Subscribe<Events::OnWindowFocus>(&OnWindowFocusTester);
	return sandbox;
}