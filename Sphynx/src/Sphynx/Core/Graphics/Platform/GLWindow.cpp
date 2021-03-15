#include "pch.h"
#define GLFW_INCLUDE_NONE
//Before IMGUI.
#include <GLFW/glfw3.h>
//IMGUI gets loaded here.
#include "GLWindow.h"
#include "glad/glad.h"
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include "GLFW/glfw3native.h"

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Events;

bool GLWindow::GLFWInit = false;

static GLWindow& GetFromGLFW(GLFWwindow* win) { return *(GLWindow*)glfwGetWindowUserPointer(win); };

void Sphynx::Core::GLWindow::mid::Resize(GLFWwindow* win, int width, int height)
{
	GLWindow& inst = GetFromGLFW(win);
	inst.Resize(width, height);
	inst.GetEventSystem()->QueueEvent<OnWindowResize>(OnWindowResize(&inst, width, height));
}

void Sphynx::Core::GLWindow::mid::Close(GLFWwindow* win)
{
	GLWindow& inst = GetFromGLFW(win);
	inst.OnClose();
}

void Sphynx::Core::GLWindow::mid::Focus(GLFWwindow* win, int value)
{
	GLWindow& inst = GetFromGLFW(win);
	//BECAUSE THIS GETS CALLED WHEN THE WINDOW CLOSES.
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

void Sphynx::Core::GLWindow::mid::KeyCapture(GLFWwindow* win, int keycode, int scancode, int action, int modifier)
{
	//Push OnKeyXXXX Events.
}

bool Sphynx::Core::GLWindow::IsAlive()
{
	return !glfwWindowShouldClose(window);
}

Sphynx::Core::GLWindow::~GLWindow()
{
	//Resource Deletion.
}

Sphynx::Core::GLWindow::GLWindow(Application* App, Bounds WinBounds, std::string title, bool fullscreen)
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
	//Create Window
	window = glfwCreateWindow(WinBounds.Width, WinBounds.Height, title.c_str(), NULL, NULL);
	if (!window) {
		Core_Error("Cannot Create Window.");
		return;
	}
	WindowsOpened += 1;

	glfwMakeContextCurrent(window);
	//Setting Up Glad.
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//Setting the glfwWindow to hold the IWindow instance
	glfwSetWindowUserPointer(window, (void*)this);
	//Setting Callbacks using a middle-man struct.
	glfwSetWindowSizeCallback(window, &mid::Resize);
	glfwSetWindowCloseCallback(window, &mid::Close);
	glfwSetWindowFocusCallback(window, &mid::Focus);
	glfwSetWindowIconifyCallback(window, &mid::Iconify);
	glfwSetWindowMaximizeCallback(window, &mid::Maximize);
	glfwSetKeyCallback(window, &mid::KeyCapture);
	//End of callbacks.

	//ToDO: Init imgui and Renderer.

	glClearColor(0.5f, 0.05f, 0.0f, 1);

}

Sphynx::Core::GLWindow::GLWindow(Application* App, Bounds WinBounds, std::string title, GLWindow* share)
{
	if (WinBounds.Height == 0 || WinBounds.Width == 0) {
		WinBounds = DefBounds;
	}
	else if (title.empty()) {
		title = "Sphynx Engine";
	}
	Init(App, WinBounds, title);
	Sharing = true;
	window = glfwCreateWindow(WinBounds.Width, WinBounds.Height, title.c_str(), NULL, share->window);
	if (!window) {
		Core_Error("Unable To Create Secondary Window");
		return;
	}
	//Setting the glfwWindow to hold the IWindow instance
	glfwSetWindowUserPointer(window, (void*)this);
	//Setting Callbacks using a middle-man struct.
	glfwSetWindowSizeCallback(window, &mid::Resize);
	glfwSetWindowCloseCallback(window, &mid::Close);
	glfwSetWindowFocusCallback(window, &mid::Focus);
	glfwSetWindowIconifyCallback(window, &mid::Iconify);
	glfwSetWindowMaximizeCallback(window, &mid::Maximize);
	glfwSetKeyCallback(window, &mid::KeyCapture);
	//End of callbacks.
}

void Sphynx::Core::GLWindow::OnClose()
{
	glfwSetWindowShouldClose(window, true);
	Close();
}

void Sphynx::Core::GLWindow::OnUpdate()
{
	if (!Sharing)
		//Handle Context Switching.
		SwitchContext(*this);
	Clear();
	glfwPollEvents();
	GetEventSystem()->DispatchImmediate<OnOverlayUpdate>(OnOverlayUpdate(this));
	glfwSwapBuffers(window);
}

void Sphynx::Core::GLWindow::OnResize(Events::OnWindowResize& e)
{
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

void Sphynx::Core::GLWindow::ChangeTitle(const char* title)
{
	glfwSetWindowTitle(window, title);
}

void Sphynx::Core::GLWindow::TerminateGLFW()
{
	glfwTerminate();
}

void Sphynx::Core::GLWindow::SwitchContext(GLWindow window)
{
	glfwMakeContextCurrent(window.window);
}

Bounds Sphynx::Core::GLWindow::GetBounds()
{
	return Bounds(Height, Width);
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