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

////Initializes the instance. Must Be call by any class that derives from IWindow.
//void Init(Application* App, Bounds WinBounds = DefBounds, std::string title = "Sphynx Engine", bool fullscreen = false)
//{
//	FullScreen = fullscreen;
//	Height = WinBounds.Height;
//	Width = WinBounds.Width;
//	Title = title;
//	OwnerEvent = *App->GetAppEventSystem();
//	//App->GetAppEventSystem()->Subscribe<IWindow, Events::OnWindowResize>(this, &IWindow::OnResize);
//	InstanceHasInit = true;
//};