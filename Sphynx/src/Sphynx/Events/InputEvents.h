#pragma once
#include "Event.h"
#include "..\KeyCode.h"

namespace Sphynx::Events {
	struct KeyEvent : Event {
	protected:
		KeyEvent(Keys keycode) :keyCode(keycode) {};
	public:
		Keys keyCode;
	};
	struct OnKeyPress : public KeyEvent {
	public:
		Mods mods;
		OnKeyPress(Keys keycode, Mods mod) : KeyEvent(keycode), mods(mod) {};
	};
	struct OnKeyRepeat : public KeyEvent {
	public:
		Mods mods;
		OnKeyRepeat(Keys keycode, Mods mod) : KeyEvent(keycode), mods(mod) {};
	};
	struct OnKeyRelease : public KeyEvent {
		OnKeyRelease(Keys keycode): KeyEvent(keycode) {}
	};
	//GLFW Specific ?
	struct OnKeyTyped : public KeyEvent {
		OnKeyTyped(Keys keycode) : KeyEvent(keycode) {};
	};
	struct MouseButtonEvent : public Event {
	protected:
		MouseButtonEvent(MouseButton _button) : button(_button) {};
	public:
		MouseButton button;
	};
	struct OnMouseButtonPress : public MouseButtonEvent {
		OnMouseButtonPress(MouseButton button) : MouseButtonEvent(button) {};
	};
	struct OnMouseButtonRelease : public MouseButtonEvent {
		OnMouseButtonRelease(MouseButton button) : MouseButtonEvent(button) {};
	};
	struct OnMouseScrollEvent : public Event {
	private:
		double X, Y;
	public:
		double GetXOffsets() { return X; }
		double GetYOffsets() { return Y; }
		OnMouseScrollEvent(double x, double y) : X(x), Y(y) {};
	};
}