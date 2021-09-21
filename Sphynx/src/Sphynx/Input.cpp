#include "pch.h"
#include "Input.h"
#include "Core/Graphics/Window.h"
#include "GLFW/glfw3.h"
#ifdef Platform_Windows
#include <Windows.h>
#endif
using namespace Sphynx::Events;

void Sphynx::Input::GLKeyHandler(GLFWwindow* window, int code, int scan, int action, int mods) {
	keyStates[code] = { (action == GLFW_PRESS ? true : false),(Mods)mods };
}
void Sphynx::Input::GLMouseHandler(GLFWwindow* window, int button, int action, int mods)
{
	MouseStates[button] = { (action == GLFW_PRESS ? true : false),(Mods)mods };
}
void Sphynx::Input::GLHandleNewWindow(Events::OnWindowOpen& e)
{
	GLFWwindow* win = reinterpret_cast<GLFWwindow*>(e.GetWindow()->GetNativePointer());
	glfwSetKeyCallback(win, &Input::GLKeyHandler);
	glfwSetMouseButtonCallback(win, &Input::GLMouseHandler);
	window = e.GetWindow();
}
void Sphynx::Input::HandleWindowClose(Events::OnWindowClose& e)
{
	window = nullptr;
	memset(keyStates, false, sizeof(keyStates));
	memset(MouseStates, false, sizeof(MouseStates));
}
void Sphynx::Input::Init()
{
	memset(keyStates, false, sizeof(keyStates));
	memset(MouseStates, false, sizeof(MouseStates));
	GetApplication()->GetAppEventSystem()->Subscribe<OnWindowClose>(&Input::HandleWindowClose);
	window = GetApplication()->GetMainWindow();
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
	case Platform::Linux:
		GetApplication()->GetAppEventSystem()->Subscribe<OnWindowOpen>(&Input::GLHandleNewWindow);
		if (GetApplication()->HasWindow()) {
			GLFWwindow* win = reinterpret_cast<GLFWwindow*>(window->GetNativePointer());
			glfwSetKeyCallback(win, &Input::GLKeyHandler);
			glfwSetMouseButtonCallback(win, &Input::GLMouseHandler);
		}
		break;
	default:
		Core_Error("Unknown or Unsupported System");
		break;
	}
}

Sphynx::KeyState Sphynx::Input::IsKeyPressed(Keys key)
{
	return keyStates[(int)key];
}

Sphynx::MouseButtonState Sphynx::Input::IsMouseButtonPressed(MouseButton button)
{
	return MouseStates[(int)button];
}

Sphynx::Vector2<double> Sphynx::Input::GetMousePosition()
{
#ifdef DX_IMPL && Platform_Windows
#else
	double Xpos, Ypos;
	glfwGetCursorPos((GLFWwindow*)window->GetNativePointer(), &Xpos, &Ypos);
	return { Xpos,Ypos };
#endif // DEBUG
}
