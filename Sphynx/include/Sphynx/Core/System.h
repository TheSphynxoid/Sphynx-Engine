#pragma once

namespace Sphynx::Core {
	//I don't know why i made this.
	class System {
	public:
		virtual ~System() = default;
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
	};
}