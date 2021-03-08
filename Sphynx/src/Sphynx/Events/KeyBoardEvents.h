#pragma once
#include "Event.h"
#include "..\KeyCode.h"

namespace Sphynx::Events {
	struct KeyEvent : Event {
	protected:
		Keys keyCode;
		KeyEvent(Keys keycode) :keyCode(keycode) {};
	};
	struct OnKeyPress :public KeyEvent {
	private:
		uint16_t RepeatCount;
	public:
		OnKeyPress(Keys keycode, uint16_t repeatcount) : KeyEvent(keycode), RepeatCount(repeatcount) {};
		uint16_t GetRepeatCount() const { return RepeatCount; }
	};
	struct OnKeyRelease : public KeyEvent {
		OnKeyRelease(Keys keycode): KeyEvent(keycode) {}
	};
	//?
	struct OnKeyTyped : public KeyEvent {
		OnKeyTyped(Keys keycode) : KeyEvent(keycode) {};
	};
}