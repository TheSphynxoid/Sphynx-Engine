#pragma once
#include "Core/Graphics/Imgui.h"
#include "GameObject.h"
#include "Events/WindowEvents.h"
#include "Core/Graphics/Pipeline/Viewport.h"

namespace Sphynx::Editor {
	class EditorViewport : public Core::IOverlayWindow
	{
	private:
		void OnResize(Events::OnWindowResize& e);
		bool ResizeFlag = true;
		Sphynx::Core::Graphics::Viewport viewport;
	public:
		EditorViewport();
		virtual void Draw() override;
	};
}