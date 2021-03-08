#pragma once
#include "Core.h"
#include "../Window.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#define IMGUI_GLFW
#include "Core/Imgui.h"


namespace Sphynx::Core {
	//GLFW implementation.
	class GLWindow final : public IWindow
	{
	private:
		inline static unsigned int WindowsOpened = 0;
		GLFWwindow* window;
		static bool GLFWInit;
		bool Vsync;
		//middle-man between GLFW and the engine.
		struct mid{
			static void Resize(GLFWwindow* win, int width, int height);
			static void Close(GLFWwindow* win);
			static void Focus(GLFWwindow* win, int value);
			static void Iconify(GLFWwindow* win, int value);
			static void Maximize(GLFWwindow* win, int value);
			static void KeyCapture(GLFWwindow* win, int keycode, int scancode, int action, int modifier);
		};
		static GLWindow& GetFromGLFW(GLFWwindow* win) { return *(GLWindow*)glfwGetWindowUserPointer(win); };
	protected:
	public:
		inline bool IsAlive()override { return !glfwWindowShouldClose(window); };
		GLWindow(Application* App, Bounds WinBounds, std::string title);
		~GLWindow()override;
		void OnClose()override;
		void OnUpdate()override;
		void OnResize(Events::OnWindowResize& e)override;
		void* GetNativePointer()override;
		Bounds GetBounds()override;
		int GetHeight()override;
		int GetWidth()override;
		inline bool IsVsyncEnabled()override { return Vsync; };
		void SetVsync(bool vsync)override;
		void ChangeTitle(const char* title)override;
		///////GLWindow Function/////////
		static void TerminateGLFW() { glfwTerminate(); };
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