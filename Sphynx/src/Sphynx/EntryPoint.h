#pragma once
#include "Core.h"
#include "Logger.h"
#ifndef EntryPoint 
#define EntryPoint
#if defined(Platform_Windows) && !defined(No_EntryPoint) && !defined(Allow_Custom_Main)
#define main main
extern Sphynx::Application* Sphynx::CreateApplication();

int main(int argc,char** argv)
{
	Sphynx::Logger::Init();
	auto app = Sphynx::CreateApplication();
	app->Run(argc, argv);
	delete app;
}
#elif defined(No_EntryPoint) && !defined(Allow_Custom_Main)
int main(int argc,char** argv)
{
	Sphynx::Logger::Init();
	auto app = Sphynx::CreateApplication();
	app->Run(argc, argv);
	delete app;
}
#define main SphynxMain
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