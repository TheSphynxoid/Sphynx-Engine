#pragma once
#include "Core/Graphics/Imgui.h"
#ifndef Editor_Core
#include "SceneManagerView.h"
#include "EditorViewport.h"
#include "GameObjectView.h"
#include "Console.h"
#endif

namespace Sphynx::Editor {
	class Editor : public Sphynx::Core::IOverlayWindow
	{
	private:
		unsigned int DockID = 0;
	public:
		Editor();
		virtual void Draw() override;
		unsigned int GetDockID() { return DockID; };

		//void AddToMainMenu()
	};
}
