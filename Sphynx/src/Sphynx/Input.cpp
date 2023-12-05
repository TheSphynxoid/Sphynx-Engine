#include "pch.h"
#include "Input.h"
#include "Core/Graphics/Window.h"
#include "GLFW/glfw3.h"
#ifdef Platform_Windows
#include <Windows.h>
#endif
using namespace Sphynx::Events;

GLFWkeyfun ImguiKeyFunc;
GLFWmousebuttonfun ImGuiMouseFunc;

void Sphynx::Input::GLKeyHandler(GLFWwindow* window, int code, int scan, int action, int mods) {
	ImguiKeyFunc(window, code, scan, action, mods);
	keyStates[code] = { (Action)action, (Mods)mods };
	switch (action)
	{
	case GLFW_RELEASE:
		GlobalEventSystem::GetInstance()->QueueEvent<OnKeyRelease>(OnKeyRelease((Keys)code));
		break;
	case GLFW_PRESS:
		GlobalEventSystem::GetInstance()->QueueEvent<OnKeyPress>(OnKeyPress((Keys)code, (Mods)mods));
		break;
	case GLFW_REPEAT:
		GlobalEventSystem::GetInstance()->QueueEvent<OnKeyRepeat>(OnKeyRepeat((Keys)code, (Mods)mods));
		break;
	default:
		Core_Warn("Unvalid action value, Key action unhandled");
		break;
	}
}

void Sphynx::Input::GLMouseHandler(GLFWwindow* window, int button, int action, int mods)
{
	MouseStates[button] = { (Action)action, (Mods)mods};
	ImGuiMouseFunc(window, button, action, mods);
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
		[[fallthrough]];
#endif
	case Platform::Android:
		[[fallthrough]];
	case Platform::Linux:
		if (GetApplication()->HasWindow()) {
			GLFWwindow* win = reinterpret_cast<GLFWwindow*>(window->GetNativePointer());
			ImguiKeyFunc = glfwSetKeyCallback(win, &Input::GLKeyHandler);
			ImGuiMouseFunc = glfwSetMouseButtonCallback(win, &Input::GLMouseHandler);
		}
		break;
	default:
		Core_Error("Unknown or Unsupported System");
		break;
	}
}

bool Sphynx::Input::IsKeyPressed(Keys key)
{
	return keyStates[(int)key];
}

bool Sphynx::Input::IsMouseButtonPressed(MouseButton button)
{
	return MouseStates[(int)button];
}

glm::vec2 Sphynx::Input::GetMousePosition()
{
#ifdef DX_IMPL && Platform_Windows
#else
	double Xpos, Ypos;
	glfwGetCursorPos((GLFWwindow*)window->GetNativePointer(), &Xpos, &Ypos);
	return { Xpos,Ypos };
#endif // DEBUG
}
