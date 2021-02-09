#include <Sphynx.h>
#include <Sphynx/Events/Event.h>
#include <Sphynx/Events/ApplicationEvents.h>
#include <iostream>

using namespace Sphynx;

struct OnFakeEvent : public Sphynx::Events::Event {

};

class SandBox : public Sphynx::Application
{
public:
	SandBox() {

	}
	~SandBox() {

	}
	void Test(OnFakeEvent& e) {
		std::cout << "ooooooooo";
	}
private:

};

void FreeTest(OnFakeEvent& e) {
	std::cout << "Fooooooooo";
}
//The Main Entry Point for clients
Sphynx::Application* Sphynx::CreateApplication() {
	auto sandbox = new SandBox();
	return sandbox;
}