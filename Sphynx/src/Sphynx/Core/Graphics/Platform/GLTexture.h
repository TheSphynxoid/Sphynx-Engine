#pragma once
#include "Core/Graphics/Pipeline/Texture.h"
namespace Sphynx::Core::Graphics::GL {
	class GLTexture : public Texture
	{
		virtual void CreateTexture()override {};
	};
}
