#pragma once

//For Convinence Sake.
namespace Sphynx{}

#ifdef Platform_Win
extern Sphynx::Application* Sphynx::CreateApplication();

int main(int argc,char** argv) {
	auto app = Sphynx::CreateApplication();
	app->Run();
	delete app;
}
#endif // Platform_Win
