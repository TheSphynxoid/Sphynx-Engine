#pragma once
#include "Event.h"
#include "Scene.h"

namespace Sphynx::Events {
	struct OnSceneChange : public Event {
		const Core::Scene& CurrentScene;
		OnSceneChange(const Sphynx::Core::Scene& newScene) : CurrentScene(newScene) {};
	};
	struct OnSceneStart : public Event {
		const Core::Scene& CurrentScene;
		OnSceneStart(const Sphynx::Core::Scene& newScene) : CurrentScene(newScene) {};
	};
	struct OnSceneUpdate : public Event {
		OnSceneUpdate() {};
	};
}