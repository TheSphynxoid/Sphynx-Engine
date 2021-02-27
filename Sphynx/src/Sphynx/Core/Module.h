#pragma once

//Forward Declarations

namespace Sphynx {
	class Application;
}

namespace Sphynx::Core {
	//enum class ModuleState : int {
	//	Idle,Working,Stalling,Waiting,Error
	//};

	//Base Class for all modules
	class Module {
	protected:
		//ModuleState State;
	public:
		Module() = default;
		virtual ~Module() {};
		virtual void Start(Application* app) = 0;
		virtual void Shutdown() {};
		virtual void Update() = 0;
		//ModuleState GetState() { return State; };
	};
}