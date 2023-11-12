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

		void Update(std::vector<System*>& list) {
			for (auto& sys : list) {
				sys->Update();
			}
		}
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
			std::vector<System*>& SystemVector = std::vector<System*>();
			switch (system->GetSystemUpdateOn())
			{
			case Sphynx::UpdateOn::PreEvents:
				SystemVector = PreEventSystems;
				break;
			case Sphynx::UpdateOn::OnUpdate:
				SystemVector = OnUpdateSystems;
				break;
			case Sphynx::UpdateOn::PrePhysics:
				SystemVector = PrePhysicsSystems;
				break;
			case Sphynx::UpdateOn::OnFixedUpdate:
				SystemVector = OnFixedSystems;
				break;
			case Sphynx::UpdateOn::PreRender:
				SystemVector = PreRenderSystems;
				break;
			case Sphynx::UpdateOn::PostRender:
				SystemVector = PostRenderSystems;
				break;
			default:
				Core_Error("System Update time Unknown");
				return;
			}
			for (unsigned int i = 0; i < SystemVector.size();i++) {
				if (SystemVector[i] == system) {
					SystemVector[i]->Shutdown();
					SystemVector.erase(SystemVector.begin() + i);
					return;
				}
			}
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

		void PreEvent() {
			Update(PreEventSystems);
		}
		void OnUpdate() {
			Update(OnUpdateSystems);
		}
		void PrePhysics() {
			Update(PrePhysicsSystems);
		}
		void FixedUpdate() {
			Update(OnFixedSystems);
		}
		void PreRender() {
			Update(PreRenderSystems);
		}
		void PostRender() {
			Update(PostRenderSystems);
		}


	};
}
