#include <Sphynx.h>
#include <Sphynx/Event/ApplicationEvents.h>
#include <iostream>


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