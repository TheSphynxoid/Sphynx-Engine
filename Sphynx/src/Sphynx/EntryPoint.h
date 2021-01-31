#pragma once
#ifdef Platform_Win
extern Sphynx::Application* Sphynx::CreateApplication();

int main(int argc,char** argv) {
	Sphynx::Logger::Init();
	auto app = Sphynx::CreateApplication();
	app->Run();
	delete app;
}
#endif // Platform_Win