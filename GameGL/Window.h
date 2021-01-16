#pragma once
#include "GLFW/glfw3.h"

class Window
{
private:
	GLFWwindow* window_ptr;
	GLFWmonitor* monitor_ptr;
	int height, width;
	bool IsAlive;
public:
	Window(int Height,int Width,const char* Title,GLFWmonitor* Monitor);
	void Update();
	void Clear();
	bool IsClosing() { return !IsAlive; }
};

