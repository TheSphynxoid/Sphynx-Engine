#include "Window.h"

Window::Window(int Height, int Width, const char* Title, GLFWmonitor* Monitor)
{
	window_ptr = glfwCreateWindow(Height, Width, Title, Monitor, NULL);
	height = Height;
	width = Width;
	monitor_ptr = Monitor;
	IsAlive = (window_ptr != NULL);
}

void Window::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(this->window_ptr);
	IsAlive = glfwWindowShouldClose(this->window_ptr);
}

void Window::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}