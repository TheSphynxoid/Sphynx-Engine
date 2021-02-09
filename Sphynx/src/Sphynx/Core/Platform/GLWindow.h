#pragma once
#include "Core.h"
#include "../Window.h"
#include "GLFW/glfw3.h"

namespace Sphynx::Core {
	//GLFW implementation.
	class GLWindow : public IWindow
	{
	private:
		GLFWwindow* window;
		static bool GLFWInit;
		struct mid{
			static void Resize(GLFWwindow* win, int width, int height);
		};
	protected:
	public:
		GLWindow(Application* App, Bounds WinBounds, std::string title);
		~GLWindow()override;
		void OnUpdate(Events::OnWindowUpdate& e)override;
		void OnResize(Events::OnWindowResize& e)override;
		void* GetNativePointer()override;
		Bounds GetBounds()override;
		int GetHeight()override;
		int GetWidth()override;
		void SetVsync(bool vsync)override;
		void Clear();

		//friend mid;
	};
}