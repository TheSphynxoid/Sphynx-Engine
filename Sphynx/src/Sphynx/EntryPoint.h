#pragma once
#if defined(Platform_Win) && !defined(No_EntryPoint)
#define main main
extern Sphynx::Application* Sphynx::CreateApplication();

int main(int argc,char** argv)
{
	Sphynx::Logger::Init();
	auto app = Sphynx::CreateApplication();
	app->Run();
	delete app;
}
#elif defined(No_EntryPoint)
#define main SphynxMain
int main(int argc,char** argv)
{
	Sphynx::Logger::Init();
	auto app = Sphynx::CreateApplication();
	app->Run();
	delete app;
}
#endif // Platform_Win