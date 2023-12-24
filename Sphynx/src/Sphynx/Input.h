#pragma once
#include "KeyCode.h"
#include "Events/InputEvents.h"
#include "Events/WindowEvents.h"
#include "Vector.h"
#include "glm/glm.hpp"

struct GLFWwindow;

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
	typedef struct _State{
		Action action;
		Mods mods;
		operator bool() {
			return action == Action::Pressed || action == Action::Repeat;
		}
	}KeyState, MouseButtonState;

	typedef bool(*KeyCallback)(Keys, int, Action, Mods);
	typedef bool(*MouseCallback)(MouseButton button, Action action, Mods mods);
	typedef void(*CursorEnterCallback)(bool Entered);
	typedef void(*CursorPositionCallback)(glm::vec2 NewPos);
	typedef void(*CharCallback)(unsigned int codepoint);
	typedef bool(*ScrollCallback)(double Xoffset, double Yoffset);

	class Input {
	private:
		//Input States.
		inline static KeyState keyStates[Last_Key];
		inline static MouseButtonState MouseStates[Last_Mouse_Button];
		inline static double ScrollX = 0, ScrollY = 0;
		inline static glm::vec2 CursorPos = glm::vec2(0,0);
		//Window Polled.
		inline static Core::IWindow* window = nullptr;
		//Callbacks.
		inline static KeyCallback KeyCB = nullptr;
		inline static MouseCallback MousePressCB = nullptr;
		inline static CursorEnterCallback CursorEnterCB = nullptr;
		inline static CursorPositionCallback CursorPosCB = nullptr;
		inline static CharCallback CharCB = nullptr;
		inline static ScrollCallback ScrollCB = nullptr;
		//Events.
		static void HandleWindowClose(Events::OnWindowClose& e);
		//GL Specific.
		static void GLKeyHandler(GLFWwindow* window, int code, int scan, int action, int mods);
		static void GLMouseHandler(GLFWwindow* window, int button, int action, int mods);
		static void GLCursorEnterHandler(GLFWwindow* window, int Entered);
		static void GLCursorPositionHandler(GLFWwindow* window, double Xpos, double Ypos);
		static void GLCharHandler(GLFWwindow* window, unsigned int codepoint);
		static void GLScrollHandler(GLFWwindow* window, double XOffset, double YOffset);
	public:
		static void Init();
		static bool IsKeyPressed(Keys key) 
		{
			return keyStates[(int)key];
		}
		//For Imgui Support.

		static KeyCallback GetKeyCallback() { return KeyCB; }
		static MouseCallback GetMouseCallback() { return MousePressCB; }
		static CursorEnterCallback GetCursorEnterCallback() { return CursorEnterCB; };
		static CursorPositionCallback GetCursorPositionCallback() { return CursorPosCB; }
		static CharCallback GetCharCallback() { return CharCB; }
		static ScrollCallback GetScrollCallback() { return ScrollCB; }
		static void InstallKeyCallback(KeyCallback cb) { KeyCB = cb; }
		static void InstallMouseCallback(MouseCallback cb) { MousePressCB = cb; }
		static void InstallScrollCallback(ScrollCallback cb) { ScrollCB = cb; }
		static void InstallCursorPositionCallback(CursorPositionCallback cb) { CursorPosCB = cb; }
		static void InstallCharCallback(CharCallback cb) { CharCB = cb; }
		static void InstallCursorEnterCallback(CursorEnterCallback cb) { CursorEnterCB = cb; }
		//A copy of ImGui_ImplGlfw_TranslateUntranslatedKey, Normal Input works for games this is for shortcuts.
		//The Engine does not use this because i do not understand the problem it fixes :) This is purely for the user right now.
		static int TranslateKey(int key, int scancode);

		//Returns a pointer to a key state.
		static KeyState GetKeyState(Keys key)
		{
			return keyStates[(int)key];
		}
		static bool IsMouseButtonPressed(MouseButton button)
		{
			return MouseStates[(int)button];
		}
		static MouseButtonState GetMouseButtonState(MouseButton button)
		{
			return MouseStates[(int)button];
		}
		static Mods GetMods(Keys key)
		{
			return keyStates[(int)key].mods;
		}
		static Mods GetMods(MouseButton button)
		{
			return MouseStates[(int)button].mods;
		}
		static glm::vec2 GetMousePosition()
		{
			return CursorPos;
		}
	};
}
