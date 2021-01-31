#pragma once
#include "Event.h"
#include "..\KeyCode.h"

namespace Sphynx::Events {
	struct KeyEvent : Event {
	protected:
		Keys keyCode;
		KeyEvent(Keys keycode) :keyCode(keycode) {};
	};
	struct OnKeyPressEvent :public KeyEvent {
	private:
		uint16_t RepeatCount;
	public:
		OnKeyPressEvent(Keys keycode, uint16_t repeatcount) : KeyEvent(keycode), RepeatCount(repeatcount) {};
		uint16_t GetRepeatCount() const { return RepeatCount; }
	};
	struct OnKeyReleaseEvent : public KeyEvent {
		OnKeyReleaseEvent(Keys keycode): KeyEvent(keycode) {}
	};
	struct OnKeyTypedEvent : public KeyEvent {
		OnKeyTypedEvent(Keys keycode) : KeyEvent(keycode) {};
	};
}