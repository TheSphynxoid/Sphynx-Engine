#include <Sphynx.h>

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
	return new SandBox();
}