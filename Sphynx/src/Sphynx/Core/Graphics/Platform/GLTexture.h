#pragma once
#include "Core/Graphics/Pipeline/Texture.h"
namespace Sphynx::Core::Graphics::GL {
	class GLTexture : public Texture
	{
	private:
		unsigned int TextureID = 0;
		unsigned int GLTextureType;
		int Width, Height;
		int Bits = 4;
		TextureFormat Format;
		TextureDataFormat DataFormat;
		TextureType Type;
		void Release();
	public:
		GLTexture(const char* path, TextureType Type, int MipmapLevel, TextureFormat format, TextureDataFormat datatype, TextureWrappingMode warp,
			TextureFilterMode filter, TextureMipmapMode MipmapMode);
		GLTexture(TextureType Type, int width, int height, int MipmapLevel, TextureFormat format, TextureDataFormat datatype, TextureWrappingMode warp,
			TextureFilterMode filter, TextureMipmapMode MipmapMode);
		GLTexture(const GLTexture&) = delete;
		~GLTexture();
		GLTexture& operator=(const GLTexture&) = delete;
		GLTexture(GLTexture&& tex)noexcept;
		GLTexture& operator=(GLTexture&& tex)noexcept;

		virtual void SetData(const unsigned char* data) override;
		virtual void Bind() override;
		virtual void Unbind() override;
		virtual const TextureFormat& GetFormat() override {
			return Format;
		}
		virtual const TextureDataFormat& GetDataFormat() override {
			return DataFormat;
		}
		virtual const TextureType& GetTextureType() override {
			return Type;
		}
		virtual int GetWidth() override { return Width; }
		virtual int GetHeight() override { return Height; }
		virtual int GetBitsPerPixel() override { return Bits; };
		virtual void* ReadAllPixels(TextureDataFormat data) override;
		virtual void* GetNativeID() override { return (void*)TextureID; };
		friend class GLFrameBuffer;
	};
}
