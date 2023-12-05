#include "pch.h"
#define GLFW_INCLUDE_NONE
//Before IMGUI.
#include <GLFW/glfw3.h>
//IMGUI gets loaded here.
#include "GLWindow.h"
#include "glad/glad.h"
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include "GLFW/glfw3native.h"
#include "Events/InputEvents.h"
#include "Core/Graphics/Platform/GLRenderer.h"

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Events;

bool GLWindow::GLFWInit = false;

#define GetFromGLFW(win) *(::Sphynx::Core::GLWindow*)glfwGetWindowUserPointer(win)

void Sphynx::Core::GLWindow::mid::Close(GLFWwindow* win)
{
	GLWindow& inst = GetFromGLFW(win);
	inst.OnClose();
}

void Sphynx::Core::GLWindow::mid::Focus(GLFWwindow* win, int value)
{
	GLWindow& inst = GetFromGLFW(win);
	//BECAUSE THIS GETS CALLED WHEN THE WINDOW CLOSES. WTF DOES IT DO THAT
	if (inst.IsAlive()) {
		if (value == GLFW_TRUE) {
			inst.OnFocus();
			inst.GetEventSystem()->QueueEvent<OnWindowFocus>(OnWindowFocus(&inst));
		}
		else {
			inst.OnFocusLoss();
			inst.GetEventSystem()->QueueEvent<OnWindowFocusLoss>(OnWindowFocusLoss(&inst));
		}
	}
}

void Sphynx::Core::GLWindow::mid::Iconify(GLFWwindow* win, int value)
{
	GLWindow& inst = GetFromGLFW(win);
	if (value == GLFW_TRUE) {
		inst.OnMinimize();
		inst.GetEventSystem()->QueueEvent<OnWindowMinimize>(OnWindowMinimize(&inst));
	}
	else {
		inst.OnRestore();
		inst.GetEventSystem()->QueueEvent<OnWindowRestore>(OnWindowRestore(&inst));
	}
}

void Sphynx::Core::GLWindow::mid::Maximize(GLFWwindow* win, int value)
{
	GLWindow& inst = GetFromGLFW(win);
	if (inst.IsAlive()) {
		if (value == GLFW_TRUE) {
			inst.OnMaximize();
			inst.GetEventSystem()->QueueEvent<OnWindowMaximize>(OnWindowMaximize(&inst));
		}
		else {
			inst.OnRestore();
			inst.GetEventSystem()->QueueEvent<OnWindowRestore>(OnWindowRestore(&inst));
		}
	}
}

void Sphynx::Core::GLWindow::mid::FrameBufferResize(GLFWwindow* win, int width, int height)
{
	glViewport(0, 0, width, height);
	auto& inst = GetFromGLFW(win);
	inst.Height = height;
	inst.Width = width;
	GlobalEventSystem::GetInstance()->QueueEvent<OnWindowResize>(OnWindowResize(&inst, width, height));
}

bool Sphynx::Core::GLWindow::IsAlive()
{
	return !glfwWindowShouldClose(window);
}

Sphynx::Core::GLWindow::~GLWindow()
{
	//Resource Deletion.
	Close();
}

Sphynx::Core::GLWindow::GLWindow(Application* App, Bounds WinBounds, std::string title, bool fullscreen):
	Height(WinBounds.Height), Width(WinBounds.Width)
{
	//Init base class.
	Init(App, WinBounds, title, fullscreen);
	Vsync = false;
	window = nullptr;

	glfwInitHint(GLFW_VERSION_MAJOR, 4);
	glfwInitHint(GLFW_VERSION_MINOR, 6);
	
	//Init GLFW
	GLFWInit = glfwInit();
	if (!GLFWInit) {
		Core_Error("Glfw couldn't initialize.");
		return;
	}

	GLFWmonitor* monitor = 0;
	if (fullscreen) {
		monitor = glfwGetPrimaryMonitor();
		//glfwSetWindowSize(window, Width, Height);
	}
	window = glfwCreateWindow(Width, Height, title.c_str(), monitor, NULL);
	//Create Window
	if (!window) {
		Core_Error("Cannot Create Window.");
		return;
	}

	glfwMakeContextCurrent(window);
	//Setting Up Glad.
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//Setting the glfwWindow to hold the IWindow instance
	glfwSetWindowUserPointer(window, (void*)this);
	//Setting Callbacks using a middle-man struct.
	glfwSetWindowCloseCallback(window, &mid::Close);
	glfwSetWindowFocusCallback(window, &mid::Focus);
	glfwSetWindowIconifyCallback(window, &mid::Iconify);
	glfwSetWindowMaximizeCallback(window, &mid::Maximize);
	glfwSetFramebufferSizeCallback(window, &mid::FrameBufferResize);
	//End of callbacks.

	glfwFocusWindow(window);

	//ToDO: Init imgui and Renderer.
	Renderer = new Core::Graphics::GL::GLRenderer();
	Renderer->Start(this);
}

Sphynx::Core::GLWindow::GLWindow(Application* App, Bounds WinBounds, std::string title, bool fullscreen, GLWindow* share) :
	Height(WinBounds.Height), Width(WinBounds.Width)
{
	if (WinBounds.Height == 0 || WinBounds.Width == 0) {
		WinBounds = DefBounds;
	}
	else if (title.empty()) {
		title = "Sphynx Engine";
	}
	Init(App, WinBounds, title);
	Sharing = true;
	glfwInitHint(GLFW_VERSION_MAJOR, 4);
	glfwInitHint(GLFW_VERSION_MINOR, 6);	
	//Init GLFW
	GLFWInit = glfwInit();
	if (!GLFWInit) {
		Core_Error("Glfw couldn't initialize.");
		return;
	}
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	GLFWmonitor* monitor = 0;
	if (fullscreen)monitor = glfwGetPrimaryMonitor();
	window = glfwCreateWindow(WinBounds.Width, WinBounds.Height, title.c_str(), monitor, share->window);
	if (!window) {
		Core_Error("Unable To Create Secondary Window");
		return;
	}

	//Setting the glfwWindow to hold the IWindow instance
	glfwSetWindowUserPointer(window, (void*)this);
	//Setting Callbacks using a middle-man struct.
	glfwSetWindowCloseCallback(window, &mid::Close);
	glfwSetWindowFocusCallback(window, &mid::Focus);
	glfwSetWindowIconifyCallback(window, &mid::Iconify);
	glfwSetWindowMaximizeCallback(window, &mid::Maximize);
	glfwSetFramebufferSizeCallback(window, &mid::FrameBufferResize);
	//End of callbacks.
	Renderer = new Core::Graphics::GL::GLRenderer();
	Renderer->Start(this);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Sphynx::Core::GLWindow::OnClose()
{
	glfwSetWindowShouldClose(window, true);
	Close();
	glfwDestroyWindow(window);
	//this->GetEventSystem()->ClearEventSubscibers<OnOverlayUpdate>();
}

void Sphynx::Core::GLWindow::OnUpdate()
{
	if (IsAlive()) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Sphynx::Core::GLWindow::OnResize(Events::OnWindowResize& e)
{
	glfwSetWindowSize(((GLFWwindow*)e.GetWindow()->GetNativePointer()), e.Width, e.Height);
	mid::FrameBufferResize(((GLFWwindow*)e.GetWindow()->GetNativePointer()), e.Width, e.Height);
}

void* Sphynx::Core::GLWindow::GetNativePointer()
{
	return window;
}

void Sphynx::Core::GLWindow::SetVsync(bool vsync)
{
	glfwSwapInterval(vsync);
	Vsync = vsync;
}

void Sphynx::Core::GLWindow::Internal_ChangeTitle(const char* title)
{
	glfwSetWindowTitle(window, title);
}

void Sphynx::Core::GLWindow::SetClearColor(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

Core::Graphics::IRenderer* Sphynx::Core::GLWindow::GetRenderer()
{
	return Renderer;
}

void Sphynx::Core::GLWindow::TerminateGLFW()
{
	glfwTerminate();
}

void Sphynx::Core::GLWindow::SwitchContext(GLWindow* window)
{
	glfwMakeContextCurrent(window->window);
}

Bounds Sphynx::Core::GLWindow::GetBounds()
{
	return { Width, Height };
}

int Sphynx::Core::GLWindow::GetHeight()
{
	return Height;
}

int Sphynx::Core::GLWindow::GetWidth()
{
	return Width;
}

void Sphynx::Core::GLWindow::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Sphynx::Core::GLWindow::OnFocus()
{

}

void Sphynx::Core::GLWindow::OnFocusLoss()
{
	//Inform the input system.
}

void Sphynx::Core::GLWindow::OnRestore()
{
	//Inform the renderer.
}

void Sphynx::Core::GLWindow::OnMinimize()
{
	//Stop Renderering (you can also pause the game too.)
}

void Sphynx::Core::GLWindow::OnMaximize()
{
}
