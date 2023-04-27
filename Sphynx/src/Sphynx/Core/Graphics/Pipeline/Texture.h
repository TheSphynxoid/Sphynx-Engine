#pragma once
#include "Buffer.h"

namespace Sphynx::Core::Graphics {
#pragma region Enums
	enum class TextureWrappingMode : unsigned {
		Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
	};
	enum class TextureFilterMode : unsigned int {
		Nearest, Linear
	};
	enum class TextureMipmapMode : unsigned int {
		NearestMipmapNearest, NearestMipmapLinear, LinearMipmapNearest, LinearMipmapLinear
	};
	//TODO:Add GL Internal Formats.
	enum class TextureFormat : unsigned int {
		Depth24_Stencil8, Red, Red8, Red8SNorm, Red16, Red16SNorm, RG, RG8, RG8SNorm, RG16, RG16SNorm, R3_G3_B2, RGB,
		RGB4, RGB5, RGB8, RGB8SNorm, RGB10, RGB12, RGB16, RGB16SNorm, RGBA, RGBA2, RGBA4, RGB5_A1, RGBA8, RGBA8SNorm, RGB10_A2, UIntRGB10_A2,
		RGBA12, RGBA16, SRGB8, SRGB8_A8, BGR, BGRA, FloatR16, FloatRG16, FloatRGB16, FloatRGBA16, FloatR32, FloatRG32, FloatRGB32, FloatRGBA32,
		FloatRG11_B10, RGB9_E5, IntRed8, UIntRed8, IntRed16, UIntRed16, IntRed32, UIntRed32, IntRG8, UIntRG8, IntRG16, UIntRG16, UIntRG32, IntRGB8,
		UIntRGB8, IntRGB16, UIntRGB16, IntRGB32, UIntRGB32, IntRGBA8, UIntRGBA8, IntRGBA16, UIntRGBA16, IntRGBA32, UIntRGBA32, CompressedRed,
		CompressedRG, CompressedRGB, CompressedRGBA, CompressedSRGB, CompressedSRGB_A, CompressedRed_RGTC1, SignedCompressedRed_RGTC1,
		CompressedRG_RGTC2, CompressedRGBA_BPTC_Unorm, CompressedSRGB_A_BPTC_UNorm, FloatCompressedRGB_BPTC, UFloatCompressedRGB_BPTC
	};
	//\I Don't know any of these formats
	enum class TextureDataFormat : unsigned int {
		UByte, Byte, UShort, Short, UInt, Int, HalfFloat, Float, UByte_3_3_2,
		UByte_2_3_3_REV, UShort_5_6_5, UShort_5_6_5_REV, UShort_4_4_4_4,
		UShort_4_4_4_4_REV, UShort_5_5_5_1, UShort_1_5_5_5_REV, UInt_8_8_8_8,
		UInt_8_8_8_8_REV, UInt_10_10_10_2, UInt_2_10_10_10_REV, UInt_24_8
	};
	enum class TextureType : unsigned int {
		Texture2D, Texture3D, CubeMap, Rectangle
	};
#pragma endregion
	class TextureBuffer : public Buffer {};
	//Represents a Texture in gpu memory.
	class Texture
	{
	protected:
		int Refs = 0;
		Texture();
	public:
		virtual ~Texture() 
		{ 
			Refs--;
			if (!Refs)MarkForCleanup(this);
		};
		//Sets the default format used when creating texture with unspecified format.
		static void SetDefaultFormat(TextureFormat format);
		//Sets the default Wrapping mode used when creating texture with unspecified Wrapping mode.
		static void SetDefaultWrappingMode(TextureWrappingMode wrapmode);
		//Sets the default Filtering used when creating texture with unspecified Filtering.
		static void SetDefaultFilterMode(TextureFilterMode filter);
		//Sets the default Mipmap mode used when creating texture with unspecified Mipmap mode.
		static void SetDefaultMipmapMode(TextureMipmapMode mipmapMode);
		//\Mipmap level 0
		//Creates a Texture interface object with the specified pixel data and dimensions.
		static Texture* Create(void* data, int width, int height, TextureType Type, TextureDataFormat datatype);
		//Creates a Texture interface object with the specified pixel data, dimensions and texture format (red,rb,rgb,etc.).
		static Texture* Create(void* data, int width, int height, TextureType Type, TextureFormat format, TextureDataFormat datatype);
		//Intializes a Texture interface with empty data (in opengl it doesn't create the texture buffer) and a specified format.
		static Texture* Create(TextureType Type, int Width, int Height, TextureFormat format, TextureDataFormat datatype);
		//Creates a Texture interface object with more control.
		static Texture* Create(void* data, int width, int height, TextureType Type, int MipmapLevel, TextureFormat format , TextureDataFormat datatype
			, TextureWrappingMode warp, TextureFilterMode filter, TextureMipmapMode MipmapMode);
		//Sets the Texture Data and sends it to the gpu (if texture resides in the gpu)
		//GL: calling this on an empty texture allocates the memory for the texture store
		virtual void SetData(void* data) = 0;
		//Binds the texture for usage (For gl interop)
		virtual void Bind() = 0;
		//Unbinds the texture (for opengl it binds the textureid = 0, an empty texture not for use).
		virtual void Unbind() = 0;
		virtual const TextureFormat& GetFormat() = 0;
		virtual const TextureDataFormat& GetDataFormat() = 0;
		virtual const TextureType& GetTextureType() = 0;
		//Still unused (Intended for Garbage collection).
		static void MarkForCleanup(Texture* tex) { /*tex->DeleteFlag = true;*/ };
		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;
		virtual int GetBitsPerPixel() = 0;
		virtual void* ReadAllPixels(TextureDataFormat data) = 0;
		//Gets the native handle (uint for opengl).
		virtual void* GetNativeID() = 0;
		//Generates Mipmaps for the texture.
		virtual void GenerateMipmaps() = 0;
		//Gets the compressed texture data.
		virtual DataBuffer GetCompressed() = 0;
		//Compresses the texture into a new texture object (if possible)
		virtual Texture* Compress() = 0;
		virtual TextureBuffer* GetTextureBuffer() = 0;
		friend class IRenderer;
	};
}