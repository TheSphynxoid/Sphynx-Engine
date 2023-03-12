#pragma once
#include "Core.h"
#include "../Window.h"

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
		int Height, Width;
		int SHeight, SWidth;
		//middle-man between GLFW and the engine.
		struct mid{
			static void Close(GLFWwindow* win);
			static void Focus(GLFWwindow* win, int value);
			static void Iconify(GLFWwindow* win, int value);
			static void Maximize(GLFWwindow* win, int value);
			static void FrameBufferResize(GLFWwindow* win, int width, int height);
		};
	public:
		bool IsAlive()override;
		inline bool IsSharingResources() { return Sharing; };
		GLWindow(Application* App, Bounds WinBounds, std::string title, bool fullscreen);
		//We Expect that window has already been open. 
		GLWindow(Application* App, Bounds WinBounds, std::string title, bool fullscreen, GLWindow* share);
		virtual ~GLWindow()override;
		virtual void OnClose()override;
		virtual void OnUpdate()override;
		virtual void OnResize(Events::OnWindowResize& e)override;
		virtual void* GetNativePointer()override;
		virtual Bounds GetBounds()override;
		virtual int GetHeight()override;
		virtual int GetWidth()override;
		virtual inline bool IsVsyncEnabled()override { return Vsync; };
		virtual void SetVsync(bool vsync)override;
		virtual void Internal_ChangeTitle(const char* title)override;
		virtual void SetClearColor(glm::vec4 color);
		virtual Core::Graphics::IRenderer* GetRenderer()override;
		///////GLWindow Function/////////

		static void TerminateGLFW();
		static void SwitchContext(GLWindow* window);
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