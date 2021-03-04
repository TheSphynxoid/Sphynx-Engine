#define EXPOSE_IMGUI_H
#include <Sphynx.h>
#include <Sphynx/Events/Event.h>
#include <Sphynx/Events/ApplicationEvents.h>
#include <iostream>
#include <string>



using namespace Sphynx;



class SandBox : public Sphynx::Application
{
public:
	SandBox() {
		//imgui.AddOverlayWindow(new Sphynx::Core::DebugWindow(this));
	}
	~SandBox() {

	}
private:

};

//The Main Entry Point for clients
Sphynx::Application* Sphynx::CreateApplication() {
	auto sandbox = new SandBox();
	return sandbox;
}