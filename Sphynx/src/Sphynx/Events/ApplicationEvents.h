#pragma once
#include "Event.h"
//#include "Core/Window.h"

namespace Sphynx::Core {
	class IWindow;
}

namespace Sphynx::Events {
	struct OnApplicationStart : public Event {
		OnApplicationStart() = default;
	};
	struct OnApplicationClose : public Event {
		OnApplicationClose() = default;
	};
	struct OnApplicationUpdate : public Event {

	};
}