
#include <iostream>
#include "GL/glew.h"
#include <gl/GL.h>
#include "GLFW/glfw3.h"
#include "Window.h"

int main(int argc, char* argv[]) {
	std::cout << "Startup" << std::endl;

	if (!glfwInit()) {
		std::cout << "GLFW couldn't Initialize \n";
	}
	Window win(800, 600, "Test", 0);
	while (!win.IsClosing()) {
		win.Clear();
		win.Update();
	}
	getchar();
}