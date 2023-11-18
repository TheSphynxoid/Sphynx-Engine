#pragma once

namespace Sphynx {
	enum class UpdateOn {
		PreEvents, OnUpdate, PrePhysics, OnFixedUpdate, PreRender, PostRender
	};
	class System {
	public:
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;

		virtual UpdateOn GetSystemUpdateOn() = 0;
	};
}