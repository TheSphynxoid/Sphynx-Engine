#include "pch.h"
#include "GLWindow.h"
#include "glad/glad.h"
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include "GLFW/glfw3native.h"

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Events;

bool GLWindow::GLFWInit = false;

void Sphynx::Core::GLWindow::mid::Resize(GLFWwindow* win, int width, int height)
{
	GLWindow& inst = GetFromGLFW(win);
	inst.Resize(width, height);
	//inst->GetEventSystem().Dispatch<OnWindowResize>(OnWindowResize(inst,width,height));
}

void Sphynx::Core::GLWindow::mid::Close(GLFWwindow* win)
{
	GLWindow& inst = GetFromGLFW(win);
	inst.OnClose();
	//inst->GetEventSystem().Dispatch<OnWindowResize>(OnWindowResize(inst,width,height));
}

void Sphynx::Core::GLWindow::mid::Focus(GLFWwindow* win, int value)
{
	GLWindow& inst = GetFromGLFW(win);
	if (value == GLFW_TRUE) {
		inst.OnFocus();
	}
	else {
		inst.OnFocusLoss();
	}
}

void Sphynx::Core::GLWindow::mid::Iconify(GLFWwindow* win, int value)
{
	GLWindow& inst = GetFromGLFW(win);
	if (value == GLFW_TRUE) {
		inst.OnMinimize();
	}
	else {
		inst.OnRestore();
	}
}

void Sphynx::Core::GLWindow::mid::Maximize(GLFWwindow* win, int value)
{
	GLWindow& inst = GetFromGLFW(win);
	if (value == GLFW_TRUE) {
		inst.OnMaximize();
	}
	else {
		inst.OnRestore();
	}
}

Sphynx::Core::GLWindow::~GLWindow()
{
	//Resource Deletion.
}

Sphynx::Core::GLWindow::GLWindow(Application* App, Bounds WinBounds, std::string title)
{
	//Init base class.
	Init(App, WinBounds, title, FullScreen);

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
	}

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

	glClearColor(0.5f, 0.05f, 0.0f, 1);

}

void Sphynx::Core::GLWindow::OnClose() 
{
	if (WindowsOpened != 1)glfwTerminate();
	else glfwSetWindowShouldClose(window, 1);
}

void Sphynx::Core::GLWindow::OnUpdate()
{
	Clear();
	glfwPollEvents();
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
