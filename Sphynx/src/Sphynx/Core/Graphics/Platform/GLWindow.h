#pragma once
#include "Core.h"
#include "../Window.h"

<<<<<<< HEAD:Sphynx/src/Sphynx/Core/Graphics/Platform/GLWindow.h

=======
>>>>>>> Dev_ComponentSystem:Sphynx/src/Sphynx/Core/Window/Platform/GLWindow.h
//Opaque Object.
struct GLFWwindow;
//Opaque
typedef GLFWwindow GLFWwindow;


namespace Sphynx::Core {
	namespace Graphics::GL {
		class GLRenderer;
	}
	//GLFW implementation.
	class GLWindow final : public IWindow
	{
	private:
		Graphics::GL::GLRenderer* Renderer;
		GLFWwindow* window;
		static bool GLFWInit;
		bool Vsync;
		bool Sharing = false;
		//middle-man between GLFW and the engine.
		struct mid{
			static void Resize(GLFWwindow* win, int width, int height);
			static void Close(GLFWwindow* win);
			static void Focus(GLFWwindow* win, int value);
			static void Iconify(GLFWwindow* win, int value);
			static void Maximize(GLFWwindow* win, int value);
		};
	public:
		bool IsAlive()override;
		inline bool IsSharingResources() { return Sharing; };
		GLWindow(Application* App, Bounds WinBounds, std::string title, bool fullscreen);
		//We Expect that window has already been open. 
<<<<<<< HEAD:Sphynx/src/Sphynx/Core/Graphics/Platform/GLWindow.h
		GLWindow(Application* App, Bounds WinBounds, std::string title, GLWindow* share);
=======
		GLWindow(Application* App, Bounds WinBounds, std::string title, bool fullscreen, GLWindow* share);
>>>>>>> Dev_ComponentSystem:Sphynx/src/Sphynx/Core/Window/Platform/GLWindow.h
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
		void Internal_ChangeTitle(const char* title)override;
<<<<<<< HEAD:Sphynx/src/Sphynx/Core/Graphics/Platform/GLWindow.h
=======
		Core::Graphics::IRenderer* GetRenderer()override;
>>>>>>> Dev_ComponentSystem:Sphynx/src/Sphynx/Core/Window/Platform/GLWindow.h
		///////GLWindow Function/////////

		static void TerminateGLFW();
		static void SwitchContext(GLWindow window);
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