#pragma once
#include "Component.h"
#include "Core/Font.h"

namespace Sphynx::Core {
	class TextRenderer : public Component
	{
	private:
		Font font;

		virtual void OnComponentAttach(GameObject* parent) override;
	public:
	};
}

