#pragma once

#ifdef Platform_Win
extern Sphynx::Application* Sphynx::CreateApplication();

int main(int argc,char** argv) {
	Sphynx::Logger::Init();
	Core_Info("Test");
	Client_Info("CTest");

	auto app = Sphynx::CreateApplication();
	app->Run();
	delete app;
}
#endif // Platform_Win
