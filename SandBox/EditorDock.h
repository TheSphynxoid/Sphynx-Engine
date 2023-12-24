#pragma once

#include <Core/Graphics/Imgui.h>

class EditorDock : public Sphynx::Core::IOverlayWindow
{
	// Inherited via IOverlayWindow
	void Draw() override;
};

