#include "pch.h"
#include "GLFWInput.h"
#include <GLFW/glfw3.h>
#include "Core/Graphics/Platform/GLWindow.h"

#define GetFromGLFW(win) *(::Sphynx::Core::GLWindow*)glfwGetWindowUserPointer(win)

void Sphynx::Core::GLFWInput::KeyCapture(GLFWwindow* win, int keycode, int scancode, int action, int modifier)
{
    auto w = GetFromGLFW(win);
    auto inp = (GLFWInput*)w.GetInput();
    for (auto de : inp->OnKeyPressCallbacks) {
        de->Invoke((Keys)keycode, (Action)action);
    }
    w.GetEventSystem()->QueueEvent<Sphynx::Events::OnKeyPress>(Events::OnKeyPress((Keys)keycode, (Mods)modifier));
}

void Sphynx::Core::GLFWInput::MouseCapture(GLFWwindow* win, int button, int action, int modifier)
{
    auto w = GetFromGLFW(win);
    auto inp = (GLFWInput*)w.GetInput();
    for (auto de : inp->OnMouseButtonCallbacks) {
        de->Invoke((MouseButton)button, (Action)action);
    }
    //w.GetEventSystem()->QueueEvent<Sphynx::Events::OnKeyPress>(Events((Keys)keycode, (Mods)modifier));
}

void Sphynx::Core::GLFWInput::MouseMove(GLFWwindow* win, double xpos, double ypos)
{
    auto w = GetFromGLFW(win);
    auto inp = (GLFWInput*)w.GetInput();
    for (auto de : inp->OnMouseMoveCallbacks) {
        de->Invoke(Vec2(xpos, ypos));
    }
}

Sphynx::Core::GLFWInput::GLFWInput(GLFWwindow* win) : window(win)
{
    glfwSetKeyCallback(win, &KeyCapture);
    glfwSetMouseButtonCallback(win, &MouseCapture);
    glfwSetCursorPosCallback(win, &MouseMove);
}

bool Sphynx::Core::GLFWInput::IsKeyPressed(Keys key)
{
    return glfwGetKey(window, (int)key);
}

bool Sphynx::Core::GLFWInput::IsMouseButtonPressed(MouseButton button)
{
    return glfwGetMouseButton(window, (int)button);
}

Sphynx::Vec2 Sphynx::Core::GLFWInput::GetMousePosition()
{
    double x, y; 
    glfwGetCursorPos(window, &x, &y);
    return Vec2((float)x, (float)y);
}
