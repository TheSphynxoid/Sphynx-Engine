#pragma once

namespace Sphynx::Core {
	//enum class ModuleState : int {
	//	Idle,Working,Stalling,Waiting,Error
	//};

	//Base Class for all modules
	class Module {
	protected:
		//ModuleState State;
	public:
		virtual ~Module() = 0;
		virtual void Start() = 0;
		virtual void Shutdown() = 0;
		virtual void Update() = 0;
		//ModuleState GetState() { return State; };
	};
}