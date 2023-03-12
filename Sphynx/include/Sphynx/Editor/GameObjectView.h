#pragma once
#include "Core/Graphics/Imgui.h"

namespace Sphynx {
	class GameObject;
}

namespace Sphynx::Editor {
	class GameObjectView : public Core::IOverlayWindow
	{
	private:
		GameObject* CurrentGO = nullptr;
	public:
		GameObjectView();
		virtual void Draw() override;
		void SetGameObjectView(GameObject* go);
	};
}