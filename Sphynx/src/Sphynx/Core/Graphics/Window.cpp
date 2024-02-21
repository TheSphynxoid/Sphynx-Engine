#include "pch.h"
#include "Window.h"
#include "Platform/GLWindow.h"

Sphynx::Core::IWindow* Sphynx::Core::IWindow::Create(Application* App, Bounds WinBounds, std::string title, bool fullscreen)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLWindow(App, WinBounds, title, fullscreen);
#endif
	case Sphynx::Platform::Linux:
		return new GLWindow(App, WinBounds, title, fullscreen);
	default:
		break;
	}
	return nullptr;
}
