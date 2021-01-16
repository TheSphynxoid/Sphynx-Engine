#pragma once

#include "Core.h"

namespace Sphynx {
	class SphynxAPI Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};
	//To be defined in a client
	Application* CreateApplication();
}

