#pragma once
#include "Core/Graphics/Imgui.h"

namespace Sphynx::Editor{
	class Console : public Core::IOverlayWindow {
	public:
		Console();
		virtual void Draw() override;
	};
}
