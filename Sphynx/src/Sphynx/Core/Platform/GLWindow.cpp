#include "pch.h"
#include "GLWindow.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include <gl/GL.h>

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Events;

bool GLWindow::GLFWInit = false;

void Sphynx::Core::GLWindow::mid::Resize(GLFWwindow* win, int width, int height)
{
	GLWindow* inst = (GLWindow*)glfwGetWindowUserPointer(win);
	//inst->Resize(width, height);
	inst->GetEventSystem().Dispatch<OnWindowResize>(OnWindowResize(inst,width,height));
}

Sphynx::Core::GLWindow::~GLWindow()
{
	glfwTerminate();
}

Sphynx::Core::GLWindow::GLWindow(Application* App, Bounds WinBounds, std::string title)
{
	//Init base class.
	Init(App, WinBounds, title, FullScreen);

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
	//
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, (void*)this);

	glfwSetWindowSizeCallback(window, &mid::Resize);

	glClearColor(0, 0.05f, 0.5f, 1);

}

void Sphynx::Core::GLWindow::OnUpdate(Events::OnWindowUpdate& e)
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