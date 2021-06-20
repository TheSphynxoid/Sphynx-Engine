#pragma once
#include "Component.h"
#include "GameObject.h"

namespace Sphynx::Core::Internal {
	class TestComponent : public Component {
	private:
		TestComponent() {};
		virtual void OnComponentCreate(GameObject* Parent) {};
		virtual void OnComponentAttach(GameObject* Parent) {};
		virtual void OnComponentDetach() {};
		virtual Component* CreateSelf() { return new TestComponent(); };
	public:
		~TestComponent() {};
		void Update() {};
	};
}