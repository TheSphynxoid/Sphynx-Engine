#pragma once
#include "KeyCode.h"
#include "Events/InputEvents.h"
#include "Vector.h"

struct GLFWwindow;

namespace Sphynx {
	namespace Core {
		class IWindow;
	}
	//Per-Window.
	//TODO: Make it static.
	class Input {
	private:
		inline static bool keyStates[Last_Key];
		inline static Core::IWindow* window = nullptr;
		static void KeyHandler(GLFWwindow* window, int code, int scan, int action, int mods);
		static void HandleNewWindow(Core::IWindow* _window);
	public:
		static void Init();
		static bool IsKeyPressed(Keys key);
		static bool IsMouseButtonPressed(MouseButton button);
		static Sphynx::Vector2<double> GetMousePosition();
	};
}
