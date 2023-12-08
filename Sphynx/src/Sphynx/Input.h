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

	class Input {
	private:
		inline static KeyState keyStates[Last_Key];
		inline static MouseButtonState MouseStates[Last_Mouse_Button];
		inline static Core::IWindow* window = nullptr;
		//GL Specific.
		static void GLKeyHandler(GLFWwindow* window, int code, int scan, int action, int mods);
		static void GLMouseHandler(GLFWwindow* window, int button, int action, int mods);
		//General.
		static void HandleWindowClose(Events::OnWindowClose& e);
	public:
		static void Init();
		static bool IsKeyPressed(Keys key);
		//For Imgui Support.

		static void InstallKeyCallback();
		static void InstallMouseCallback();
		//A copy of ImGui_ImplGlfw_TranslateUntranslatedKey, Normal Input works for games this is for shortcuts.
		//The Engine does not use this because i do not understand the problem it fixes :) This is purely for the user right now.
		static int TranslateKey(int key, int scancode);

		//Returns a pointer to a key state.
		static KeyState GetKeyState(Keys key)
		{
			return keyStates[(int)key];
		}
		static bool IsMouseButtonPressed(MouseButton button);
		static MouseButtonState GetMouseButtonState(MouseButton button) {
			return MouseStates[(int)button];
		}
		static Mods GetMods(Keys key)
		{
			return keyStates[(int)key].mods;
		}
		static Mods GetMods(MouseButton button) {
			return MouseStates[(int)button].mods;
		}
		static glm::vec2 GetMousePosition();

		//Gets the scancode of the current key being processed.
		static inline int GetCurrentKeyScanCode();
	};
}
