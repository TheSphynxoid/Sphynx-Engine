#include <Sphynx.h>
#include <Sphynx/Events/Event.h>
#include <Sphynx/Events/ApplicationEvents.h>
#include <iostream>

using namespace Sphynx;

class window : public Core::IOverlayWindow {
	void Draw()override
	{
		
	};
};

class SandBox : public Sphynx::Application
{
public:
	SandBox() {

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