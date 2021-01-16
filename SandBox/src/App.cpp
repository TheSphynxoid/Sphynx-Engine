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

Sphynx::Application* Sphynx::CreateApplication() {
	return new SandBox();
}