#pragma once
#include "System.h"

namespace Sphynx {
	class Engine final
	{
	private:
		static std::vector<System*> PreEventSystems;
		static std::vector<System*> OnUpdateSystems;
		static std::vector<System*> PrePhysicsSystems;
		static std::vector<System*> OnFixedSystems;
		static std::vector<System*> PreRenderSystems;
		static std::vector<System*> PostRenderSystems;
	public:
		static void AddSystem(System* system) {
			switch (system->GetSystemUpdateOn())
			{
			case Sphynx::UpdateOn::PreEvents:
				PreEventSystems.push_back(system);
				break;
			case Sphynx::UpdateOn::OnUpdate:
				OnUpdateSystems.push_back(system);
				break;
			case Sphynx::UpdateOn::PrePhysics:
				PrePhysicsSystems.push_back(system);
				break;
			case Sphynx::UpdateOn::OnFixedUpdate:
				OnFixedSystems.push_back(system);
				break;
			case Sphynx::UpdateOn::PreRender:
				PreRenderSystems.push_back(system);
				break;
			case Sphynx::UpdateOn::PostRender:
				PostRenderSystems.push_back(system);
				break;
			default:
				Core_Error("System Update time Unknown");
				break;
			}
		}
		static void RemoveSystem(System* system) {

		}
		//let's hope this works
		template<typename T>
		static std::vector<T> GetSystemsByType() {
			std::vector<T> rt;
			for (auto sys : Systems) {
				if (typeid(T) == typeid(*sys)) {
					rt.push_back(sys);
				}
			}
			return rt;
		}
	};
}
