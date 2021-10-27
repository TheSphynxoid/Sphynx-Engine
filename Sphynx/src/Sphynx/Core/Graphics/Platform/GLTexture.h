#pragma once
#include "Core/Graphics/Pipeline/Texture.h"
namespace Sphynx::Core::Graphics::GL {
	class GLTexture : public Texture
	{
	private:
		unsigned int TextureID = 0;
		unsigned int GLTextureType;
		void Release();
	public:
		GLTexture(const char* path, TextureType Type, int MipmapLevel, TextureFormat format, TextureWrappingMode warp,
			TextureFilterMode filter, TextureMipmapMode MipmapMode);
		GLTexture(const GLTexture&) = delete;
		~GLTexture();
		GLTexture& operator=(const GLTexture&) = delete;
		GLTexture(GLTexture&& tex)noexcept;
		GLTexture& operator=(GLTexture&& tex);

		virtual void SetData(const unsigned char* data) override;
		virtual void Bind() override;
		virtual void Unbind() override;
	};
}
