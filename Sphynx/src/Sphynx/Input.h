#pragma once
#include "KeyCode.h"
#include "Events/InputEvents.h"
#include "Events/WindowEvents.h"
#include "Vector.h"

struct GLFWwindow;

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
	typedef struct _State{
		bool IsPressed;
		Mods mods;
		operator bool() {
			return IsPressed;
		}
	}KeyState, MouseButtonState;
	//Per-Window.
	//TODO: Make it static.
	class Input {
	private:
		inline static KeyState keyStates[Last_Key];
		inline static MouseButtonState MouseStates[Last_Mouse_Button];
		inline static Core::IWindow* window = nullptr;
		//GL Specific.
		static void GLKeyHandler(GLFWwindow* window, int code, int scan, int action, int mods);
		static void GLMouseHandler(GLFWwindow* window, int button, int action, int mods);
		static void GLHandleNewWindow(Events::OnWindowOpen& e);
		//General.
		static void HandleWindowClose(Events::OnWindowClose& e);
	public:
		static void Init();
		static KeyState IsKeyPressed(Keys key);
		static MouseButtonState IsMouseButtonPressed(MouseButton button);
		static Sphynx::Vector2<double> GetMousePosition();
	};
}
