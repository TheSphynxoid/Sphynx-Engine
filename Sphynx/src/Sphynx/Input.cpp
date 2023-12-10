#include "pch.h"
#include "Input.h"
#include "Core/Graphics/Window.h"
#include "GLFW/glfw3.h"
#ifdef Platform_Windows
#include <Windows.h>
#endif

using namespace Sphynx::Events;

void Sphynx::Input::GLKeyHandler(GLFWwindow* window, int code, int scan, int action, int mods) {
	//if KeyCB not null and returns true.
	if (KeyCB != nullptr && KeyCB((Keys)code, scan, (Action)action, (Mods)mods))return;
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
	if (MousePressCB && MousePressCB((MouseButton)button, (Action)action, (Mods)mods))return;
	MouseStates[button] = { (Action)action, (Mods)mods};
	Core_Info("aaa");
	switch (action)
	{
	case GLFW_RELEASE:
		GlobalEventSystem::GetInstance()->QueueEvent<OnMouseButtonRelease>(OnMouseButtonRelease((MouseButton)button));
		break;
	case GLFW_PRESS:
		GlobalEventSystem::GetInstance()->QueueEvent<OnMouseButtonPress>(OnMouseButtonPress((MouseButton)button));
		break;
	//case GLFW_REPEAT:
	//	GlobalEventSystem::GetInstance()->QueueEvent<OnMouseButtonRepeat>(OnMouseButtonRepeat((MouseButton)button));
	//	break;
	default:
		Core_Warn("Unvalid action value, Key action unhandled");
		break;
	}
}

void Sphynx::Input::GLCursorEnterHandler(GLFWwindow* window, int Entered)
{
	if(CursorEnterCB)CursorEnterCB(Entered);
}

void Sphynx::Input::GLCursorPositionHandler(GLFWwindow* window, double Xpos, double Ypos)
{
	//This is done this way to allow to get the old Cursor Postion.
	glm::vec2 newPos = { Xpos,Ypos };
	if(CursorPosCB)CursorPosCB(newPos);
	CursorPos = newPos;
}

void Sphynx::Input::GLCharHandler(GLFWwindow* window, unsigned int codepoint)
{
	if (CharCB)CharCB(codepoint);
}

void Sphynx::Input::GLScrollHandler(GLFWwindow* window, double XOffset, double YOffset)
{
	if (ScrollCB != nullptr && ScrollCB(XOffset, YOffset))return;
	ScrollX = XOffset;
	ScrollY = YOffset;
	GlobalEventSystem::GetInstance()->QueueEvent<OnMouseScrollEvent>(OnMouseScrollEvent(XOffset, YOffset));
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
			glfwSetKeyCallback(win, &Input::GLKeyHandler);
			glfwSetMouseButtonCallback(win, &Input::GLMouseHandler);
			glfwSetCursorEnterCallback(win, &Input::GLCursorEnterHandler);
			glfwSetCursorPosCallback(win, &Input::GLCursorPositionHandler);
			glfwSetScrollCallback(win, &Input::GLScrollHandler);
			glfwSetCharCallback(win, &Input::GLCharHandler);
		}
		break;
	default:
		Core_Error("Unknown or Unsupported System");
		break;
	}
}

int Sphynx::Input::TranslateKey(int key, int scancode)
{
	// GLFW 3.1+ attempts to "untranslate" keys, which goes the opposite of what every other framework does, making using lettered shortcuts difficult.
	// (It had reasons to do so: namely GLFW is/was more likely to be used for WASD-type game controls rather than lettered shortcuts, but IHMO the 3.1 change could have been done differently)
	// See https://github.com/glfw/glfw/issues/1502 for details.
	// Adding a workaround to undo this (so our keys are translated->untranslated->translated, likely a lossy process).
	// This won't cover edge cases but this is at least going to cover common cases.
	if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_EQUAL)
		return key;
	GLFWerrorfun prev_error_callback = glfwSetErrorCallback(nullptr);
	const char* key_name = glfwGetKeyName(key, scancode);
	glfwSetErrorCallback(prev_error_callback);
	if (key_name && key_name[0] != 0 && key_name[1] == 0)
	{
		const char char_names[] = "`-=[]\\,;\'./";
		const int char_keys[] = { GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_MINUS, GLFW_KEY_EQUAL, GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_COMMA, GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE, GLFW_KEY_PERIOD, GLFW_KEY_SLASH, 0 };
		if (key_name[0] >= '0' && key_name[0] <= '9') { key = GLFW_KEY_0 + (key_name[0] - '0'); }
		else if (key_name[0] >= 'A' && key_name[0] <= 'Z') { key = GLFW_KEY_A + (key_name[0] - 'A'); }
		else if (key_name[0] >= 'a' && key_name[0] <= 'z') { key = GLFW_KEY_A + (key_name[0] - 'a'); }
		else if (const char* p = strchr(char_names, key_name[0])) { key = char_keys[p - char_names]; }
	}
	// if (action == GLFW_PRESS) printf("key %d scancode %d name '%s'\n", key, scancode, key_name);
	return key;
}
