#pragma once
#include "Core/Graphics/Pipeline/Texture.h"
namespace Sphynx::Core::Graphics::GL {
	class GLBufferTexture {

	};
	class GLTexture : public Texture
	{
	private:
		unsigned int TextureID = 0;
		unsigned int GLTextureType;
		int Width, Height, Depth;
		int Bits = 4;
		int MipMapLevel = 0;
		TextureFormat Format;
		TextureDataFormat DataFormat;
		TextureType Type;
		void Release();
	public:
		GLTexture(void* data, int width, int height, int depth, TextureType Type, int MipmapLevel, TextureFormat format, TextureDataFormat datatype, TextureWrappingMode warp,
			TextureFilterMode filter, TextureMipmapMode MipmapMode);
		GLTexture(TextureType Type, int width, int height, int depth, int MipmapLevel, TextureFormat format, TextureDataFormat datatype, TextureWrappingMode warp,
			TextureFilterMode filter, TextureMipmapMode mipmapMode);
		GLTexture(const GLTexture&) = delete;
		~GLTexture();
		GLTexture& operator=(const GLTexture&) = delete;
		GLTexture(GLTexture&& tex)noexcept;
		GLTexture& operator=(GLTexture&& tex)noexcept;

		virtual void SetData(void* data, int Level = 0, int OffsetX = 0, int OffsetY = 0, int OffsetZ = 0, int Width = -1, int Height = -1, int Depth = 1) override;
		virtual void Resize(int Width, int Height, int Depth, int Level = 0,
			TextureFormat format = (TextureFormat)-1, TextureDataFormat dataFormat = (TextureDataFormat)-1, const void* data = 0) override;
		virtual void Clear(int level, int OffsetX, int OffsetY, int OffsetZ, int Depth, int Width, int Height,
			TextureFormat format = (TextureFormat)-1, TextureDataFormat dataFormat = (TextureDataFormat)-1, const void* data = 0) override;
		virtual void Clear(int Level, TextureFormat format, TextureDataFormat dataFormat, const void* data) override;
		virtual void Clear(int Level) override;
		virtual void Bind()noexcept override;
		virtual void Unbind()noexcept override;
		virtual const TextureFormat& GetFormat()const noexcept override {
			return Format;
		}
		virtual const TextureDataFormat& GetDataFormat()const noexcept override {
			return DataFormat;
		}
		virtual const TextureType& GetTextureType()const noexcept override {
			return Type;
		}
		virtual int GetWidth()const noexcept override { return Width; }
		virtual int GetHeight()const noexcept override { return Height; }
		virtual int GetDepth()const noexcept override { return Depth; };
		virtual int GetBitsPerPixel()const noexcept override { return Bits; };
		virtual void* ReadAllPixels(TextureDataFormat data) override;
		virtual void* GetNativeID()noexcept override { return (void*)TextureID; };
		virtual void GenerateMipmaps() override;
		virtual DataBuffer GetCompressed() override;
		virtual Texture* Compress() override;
		virtual void CopyInto(Texture* tex, int height, int width, int depth = 0, int SrcMipLevel = 0, 
			int SrcX = 0, int SrcY = 0, int SrcZ = 0, int DstMipLevel = 0, int DstX = 0, int DstY = 0, int DstZ = 0) override;
		friend class GLFrameBuffer;
	};
}
