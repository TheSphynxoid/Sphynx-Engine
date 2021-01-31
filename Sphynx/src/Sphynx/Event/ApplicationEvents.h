#pragma once
#include "Event.h"

namespace Sphynx::Events {
	struct OnApplicationStart : public Event {
		OnApplicationStart() = default;
	};
	struct OnApplicationClose : public Event {
		OnApplicationClose() = default;
	};
	struct OnApplicationResize :public Event {
		int X, Y;
		OnApplicationResize(int x, int y) :X(x), Y(y) {};
	};
	struct OnApplicationUpdate : public Event {

	};
}