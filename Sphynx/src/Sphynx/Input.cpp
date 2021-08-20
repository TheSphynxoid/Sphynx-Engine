#include "pch.h"
#include "Input.h"
#include "Core/Graphics/Window.h"
#include "GLFW/glfw3.h"
#ifdef Platform_Windows
#include <Windows.h>
#endif
using namespace Sphynx::Events;

void Sphynx::Input::KeyHandler(GLFWwindow* window, int code, int scan, int action, int mods) {
	keyStates[code] = (action == GLFW_PRESS ? true : false);
}
void Sphynx::Input::HandleNewWindow(Core::IWindow* _window)
{
	GLFWwindow* win = reinterpret_cast<GLFWwindow*>(window->GetNativePointer());
	glfwSetKeyCallback(win, &Input::KeyHandler);
	window = window;
}
void Sphynx::Input::Init()
{
	memset(keyStates, false, sizeof(keyStates));
	switch (CurrentPlatform)
	{
#ifdef DX_IMPL
	case Platform::Windows:
		break;
#error "Implement Input for windows."
#else
	case Platform::Windows:
		Core_Warn("Using OpenGL for Windows. Reminder:Add DirectX11");
		[[fallthrough]];
#endif
	case Platform::Android:
		[[fallthrough]];
	case Platform::Mac:
		[[fallthrough]];
	case Platform::Linux:
		if (GetApplication()->HasWindow()) {
			HandleNewWindow(GetApplication()->GetMainWindow());
		}
		break;
	default:
		break;
	}
}

bool Sphynx::Input::IsKeyPressed(Keys key)
{
#if DX_IMPL
#else
	return glfwGetKey((GLFWwindow*)window->GetNativePointer(), (int)key) == GLFW_RELEASE ? false : true;
#endif
}

bool Sphynx::Input::IsMouseButtonPressed(MouseButton button)
{
#ifdef DX_IMPL
#else
	return glfwGetMouseButton((GLFWwindow*)window->GetNativePointer(),(int)button) == GLFW_RELEASE ? false : true;
#endif
}

Sphynx::Vector2<double> Sphynx::Input::GetMousePosition()
{
#ifdef DX_IMPL
#else
	double Xpos, Ypos;
	glfwGetCursorPos((GLFWwindow*)window->GetNativePointer(), &Xpos, &Ypos);
	return { Xpos,Ypos };
#endif // DEBUG
}
