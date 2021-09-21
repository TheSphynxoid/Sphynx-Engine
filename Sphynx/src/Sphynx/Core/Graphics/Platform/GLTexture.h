#pragma once
#include "Core/Graphics/Pipeline/Texture.h"
namespace Sphynx::Core::Graphics::GL {
	class GLTexture : public Texture
	{
	private:
		unsigned int TextureID = 0;
		void Release();
	public:
		GLTexture();
		GLTexture(const GLTexture&) = delete;
		GLTexture& operator=(const GLTexture&) = delete;
		GLTexture(GLTexture&& tex);
		GLTexture& operator=(GLTexture&& tex);
	};
}
