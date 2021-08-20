#pragma once
#ifndef EntryPoint 
#define EntryPoint
#include <setjmp.h>
#if defined(Platform_Windows) && !defined(No_EntryPoint) && !defined(Allow_Custom_Main)
#define main main
extern Sphynx::Application* Sphynx::CreateApplication();

int main(int argc,char** argv)
{
	Sphynx::Logger::Init();
	auto app = Sphynx::CreateApplication();
	app->Run();
	delete app;
}
#elif defined(No_EntryPoint) && !defined(Allow_Custom_Main)
#define main SphynxMain
int main(int argc,char** argv)
{
	Sphynx::Logger::Init();
	auto app = Sphynx::CreateApplication();
	app->Run();
	delete app;
}
#elif defined(Allow_Custom_Main)
int main(int argc, char** argv)
{
	Sphynx::Logger::Init();
	return _main_(argc, argv);
}
#define main _main_
#endif // Platform_Windows
#else
#endif // !EntryPoint