#pragma once
#include "Core.h"
#include "../Window.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Sphynx::Core {
	//GLFW implementation.
	class GLWindow final : public IWindow
	{
	private:
		inline static unsigned int WindowsOpened = 0;
		GLFWwindow* window;
		static bool GLFWInit;
		//middle-man between GLFW and the engine.
		struct mid{
			static void Resize(GLFWwindow* win, int width, int height);
			static void Close(GLFWwindow* win);
			static void Focus(GLFWwindow* win, int value);
			static void Iconify(GLFWwindow* win, int value);
			static void Maximize(GLFWwindow* win, int value);
		};
		static GLWindow& GetFromGLFW(GLFWwindow* win) { return *(GLWindow*)glfwGetWindowUserPointer(win); };
	protected:
	public:
		GLWindow(Application* App, Bounds WinBounds, std::string title);
		~GLWindow()override;
		void OnClose()override;
		void OnUpdate()override;
		void OnResize(Events::OnWindowResize& e)override;
		void* GetNativePointer()override;
		Bounds GetBounds()override;
		int GetHeight()override;
		int GetWidth()override;
		void SetVsync(bool vsync)override;

		///////GLWindow Function/////////

		//Clears Window.
		void Clear();
		//Window Gained Input Focus.
		void OnFocus();
		//Window Lost Input Focus.
		void OnFocusLoss();
		//Window Was Restored.
		void OnRestore();
		//Window Was Minimized.
		void OnMinimize();
		//Window Was Maximized.
		void OnMaximize();

		//friend mid;
	};
}