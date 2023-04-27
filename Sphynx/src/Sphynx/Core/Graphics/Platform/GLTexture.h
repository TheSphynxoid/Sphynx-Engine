#pragma once
#include "Core/Graphics/Pipeline/Texture.h"
namespace Sphynx::Core::Graphics::GL {
	struct GLTextureBuffer : public TextureBuffer {
		unsigned int _Ownerid = 0;
		GLTextureBuffer() = default;
		GLTextureBuffer(unsigned int id) : _Ownerid(id) {};
		~GLTextureBuffer() {
			if (isMapped)Unmap();
		}
		virtual void* Map(const MapAccess& access);
		virtual void Unmap() noexcept;
	private:
		bool isMapped = false;
	};
	class GLTexture : public Texture
	{
	private:
		unsigned int TextureID = 0;
		unsigned int GLTextureType;
		int Width, Height;
		int Bits = 4;
		int MipMapLevel = 0;
		TextureFormat Format;
		TextureDataFormat DataFormat;
		TextureType Type;
		GLTextureBuffer TexBuffer;
		void Release();
	public:
		GLTexture(void* data, int width, int height, TextureType Type, int MipmapLevel, TextureFormat format, TextureDataFormat datatype, TextureWrappingMode warp,
			TextureFilterMode filter, TextureMipmapMode MipmapMode);
		GLTexture(TextureType Type, int width, int height, int MipmapLevel, TextureFormat format, TextureDataFormat datatype, TextureWrappingMode warp,
			TextureFilterMode filter, TextureMipmapMode MipmapMode);
		GLTexture(const GLTexture&) = delete;
		~GLTexture();
		GLTexture& operator=(const GLTexture&) = delete;
		GLTexture(GLTexture&& tex)noexcept;
		GLTexture& operator=(GLTexture&& tex)noexcept;

		virtual void SetData(void* data) override;
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
		virtual void GenerateMipmaps() override;
		virtual DataBuffer GetCompressed() override;
		virtual Texture* Compress() override;
		virtual TextureBuffer* GetTextureBuffer() override;
		friend class GLFrameBuffer;
	};
}
