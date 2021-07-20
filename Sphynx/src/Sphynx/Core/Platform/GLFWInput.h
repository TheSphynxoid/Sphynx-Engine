#pragma once
#include "Input.h"
#include "Delegate.h"
//Opaque Object.
struct GLFWwindow;

namespace Sphynx::Core {
	class GLFWInput : public Input
	{
	private:
		GLFWwindow* window;
		std::list<BaseDelegate<void, Keys, Action>*> OnKeyPressCallbacks;
		std::list<BaseDelegate<void, MouseButton, Action>*> OnMouseButtonCallbacks;
		std::list<BaseDelegate<void, Vec2>*> OnMouseMoveCallbacks;
		static void KeyCapture(GLFWwindow* win, int keycode, int scancode, int action, int modifier);
		static void MouseCapture(GLFWwindow* win, int button, int action, int modifier);
		static void MouseMove(GLFWwindow* win, double xpos, double ypos);
	public:
		GLFWInput(GLFWwindow* win);
		bool IsKeyPressed(Keys key)override;
		bool IsMouseButtonPressed(MouseButton button)override;
		Vec2 GetMousePosition()override;
		//Direct Registring.
		//Use Events, it's better
		template<class T>
		void RegisterKeypressCallback(Delegate<void, T, Keys, Action> del) {
			OnKeyPressCallbacks.push_back(new Delegate<void, T, Keys, Action>(del));
		}
		template<class T>
		void RegisterMouseButtonCallback(Delegate<void, T, MouseButton, Action> del) {
			OnMouseButtonCallbacks.push_back(new Delegate<void, T, MouseButton, Action>(del));
		}
		template<class T>
		void RegisterMouseMoveCallback(Delegate<void, T, Vec2> del) {
			OnMouseMoveCallbacks.push_back(new Delegate<void, T, Vec2>(del));
		}
	};
}
