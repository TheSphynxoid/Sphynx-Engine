#pragma once

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
	//TODO: After implementing this, implement compression. 
	//ref: glCompressedTexImage2D (https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCompressedTexImage2D.xhtml)
	class Texture
	{
	protected:
		//inline static std::map<std::string, Texture*> OpenTextures;
		bool DeleteFlag = false;
		int Refs = 0;
	public:
		virtual ~Texture() { Refs--; };
		static void SetDefaultFormat(TextureFormat format);
		static void SetDefaultWrappingMode(TextureWrappingMode wrapmode);
		static void SetDefaultFilterMode(TextureFilterMode filter);
		static void SetDefaultMipmapMode(TextureMipmapMode mipmapMode);
		//\Mipmap level 0
		static Texture* Create(const char* path, TextureType Type, TextureDataFormat datatype);
		static Texture* Create(TextureType Type, int Width, int Height, TextureFormat format, TextureDataFormat datatype);
		static Texture* Create(const char* path, TextureType Type, int MipmapLevel, TextureFormat format , TextureDataFormat datatype
			, TextureWrappingMode warp, TextureFilterMode filter, TextureMipmapMode MipmapMode);
		virtual void SetData(const unsigned char* data) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual const TextureFormat& GetFormat() = 0;
		virtual const TextureDataFormat& GetDataFormat() = 0;
		virtual const TextureType& GetTextureType() = 0;
		void MarkForCleanup() { DeleteFlag = true; };
		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;
		virtual int GetBitsPerPixel() = 0;
		virtual void* ReadAllPixels(TextureDataFormat data) = 0;
		virtual void* GetNativeID() = 0;
		friend class IRenderer;
	};
}