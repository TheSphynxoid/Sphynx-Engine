#pragma once
#include "Buffer.h"
#define NoOffset 0

namespace Sphynx::Core::Graphics {
#pragma region Enums
	enum class TextureWrappingMode : unsigned short {
		Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
	};
	enum class TextureFilterMode : unsigned short {
		Nearest, Linear
	};
	enum class TextureMipmapMode : unsigned short {
		NearestMipmapNearest, NearestMipmapLinear, LinearMipmapNearest, LinearMipmapLinear
	};
	//TODO:Add GL Internal Formats.
	enum class TextureFormat : unsigned short {
		Stencil,Depth_Component16, Depth_Component24, Depth_Component32, Depth_Component32F, Depth24_Stencil8, Depth32F_Stencil8,
		Red, Red8, Red8SNorm, Red16, Red16SNorm, RG, RG8, RG8SNorm, RG16,
		RG16SNorm, R3_G3_B2, RGB565, RGB, RGB4, RGB5, RGB8, RGB8SNorm, RGB10, RGB12, RGB16, RGB16SNorm, RGBA, RGBA2, RGBA4, RGB5_A1, RGBA8,
		RGBA8SNorm, RGB10_A2, UIntRGB10_A2, RGBA12, RGBA16, SRGB8, SRGB8_A8, BGR, BGRA, FloatR16, FloatRG16, 
		FloatRGB16, FloatRGBA16, FloatR32, FloatRG32, FloatRGB32, FloatRGBA32, FloatRG11_B10, RGB9_E5, IntRed8, UIntRed8, IntRed16, 
		UIntRed16, IntRed32, UIntRed32, IntRG8, UIntRG8, IntRG16, UIntRG16, UIntRG32, IntRGB8, UIntRGB8, IntRGB16, UIntRGB16, 
		IntRGB32, UIntRGB32, IntRGBA8, UIntRGBA8, IntRGBA16, UIntRGBA16, IntRGBA32, UIntRGBA32, CompressedRed, CompressedRG, CompressedRGB,
		CompressedRGBA, CompressedSRGB, CompressedSRGB_A, CompressedRed_RGTC1, SignedCompressedRed_RGTC1, CompressedRG_RGTC2, 
		CompressedRGBA_BPTC_Unorm, CompressedSRGB_A_BPTC_UNorm, FloatCompressedRGB_BPTC, UFloatCompressedRGB_BPTC, 
	};
	//
	enum class TextureDataFormat : unsigned short {
		UByte, Byte, UShort, Short, UInt, Int, HalfFloat, Float, UByte_3_3_2,
		UByte_2_3_3_REV, UShort_5_6_5, UShort_5_6_5_REV, UShort_4_4_4_4,
		UShort_4_4_4_4_REV, UShort_5_5_5_1, UShort_1_5_5_5_REV, UInt_8_8_8_8,
		UInt_8_8_8_8_REV, UInt_10_10_10_2, UInt_2_10_10_10_REV, UInt_24_8
	};
	enum class TextureType : unsigned short {
		Texture1D, Texture1D_Array, Texture2D, Texture2D_Array, Texture3D, CubeMap, Rectangle
	};
#pragma endregion
	//TODO : Bindless Texturing, Texture streaming, Buffer Orphaning.
	//Texture Interface.
	class Texture
	{
	protected:
		int Refs = 0;
		Texture() noexcept;
	public:
		virtual ~Texture() noexcept
		{ 
			Refs--;
			if (!Refs)MarkForCleanup(this);
		};
		//Sets the default format used when creating texture with unspecified format.
		static void SetDefaultFormat(TextureFormat format)noexcept;
		//Sets the default Wrapping mode used when creating texture with unspecified Wrapping mode.
		static void SetDefaultWrappingMode(TextureWrappingMode wrapmode)noexcept;
		//Sets the default Filtering used when creating texture with unspecified Filtering.
		static void SetDefaultFilterMode(TextureFilterMode filter)noexcept;
		//Sets the default Mipmap mode used when creating texture with unspecified Mipmap mode.
		static void SetDefaultMipmapMode(TextureMipmapMode mipmapMode)noexcept;
		static TextureWrappingMode GetDefaultWrappingMode()noexcept;
		static TextureFilterMode GetDefaultFilterMode()noexcept;
		static TextureMipmapMode GetDefaultMipmapMode()noexcept;
		//\Mipmap level 0
		//Creates a Texture interface object with the specified pixel data and dimensions.
		static Texture* Create(void* data, int width, int height, int depth , TextureType Type, TextureDataFormat datatype);
		//Creates a Texture interface object with the specified pixel data, dimensions and texture format (red,rb,rgb,etc.).
		static Texture* Create(void* data, int width, int height, int depth, TextureType Type, TextureFormat format, TextureDataFormat datatype);
		//Intializes a Texture interface with empty data (in opengl it doesn't create the texture buffer) and a specified format.
		static Texture* Create(TextureType Type, int Width, int Height, int depth, TextureFormat format, TextureDataFormat datatype);
		//Creates a Texture interface object with more control.
		static Texture* Create(void* data, int width, int height, int depth, TextureType Type, int MipmapLevel, TextureFormat format , TextureDataFormat datatype
			, TextureWrappingMode warp, TextureFilterMode filter, TextureMipmapMode MipmapMode);

		//Texture With Immutable Storage.
		//static Texture* CreateImmutable();
		//virtual bool IsImmutable() const noexcept = 0;
		
		//Sets the Texture Data and sends it to the gpu (if texture resides in the gpu)
		//This Assumes that the data is of the correct format and size (over size shouldn't cause a problem in Opengl) and does not do checks.
		//If Height and width are larger then the textures Dimensions then the texture storage will be reallocated then data will be sent.
		virtual void SetData(void* data,int Level = 0, int OffsetX = 0, int OffsetY = 0, int OffsetZ = 0, 
			int Width = -1, int Height = -1, int Depth = -1) = 0;
		//This will reallocate the texture and reset it to a value specified by data. 
		virtual void Resize(int Width, int Height, int Depth, int Level = 0,
			TextureFormat format = (TextureFormat)-1, TextureDataFormat dataformat = (TextureDataFormat)-1, const void* data = 0) = 0;
		virtual void Clear(int level, int OffsetX, int OffsetY, int OffsetZ, int Depth, int Width, int Height,
			TextureFormat format = (TextureFormat)-1, TextureDataFormat dataformat = (TextureDataFormat)-1, const void* data = 0) = 0;
		virtual void Clear(int level, TextureFormat format, TextureDataFormat dataformat, const void* data) = 0;
		virtual void Clear(int level) = 0;
		//Binds the texture for usage.
		virtual void Bind()noexcept = 0;
		//Unbinds the texture (for opengl it binds the textureid = 0, an empty texture not for use).
		virtual void Unbind()noexcept = 0;
		virtual const TextureFormat& GetFormat()const noexcept = 0;
		virtual const TextureDataFormat& GetDataFormat()const noexcept = 0;
		virtual const TextureType& GetTextureType()const noexcept = 0;
		//Still unused (Intended for Garbage collection).
		static void MarkForCleanup(Texture* tex) { /*tex->DeleteFlag = true;*/ };
		virtual int GetWidth()const noexcept = 0;
		virtual int GetHeight()const noexcept = 0;
		virtual int GetDepth()const noexcept = 0;
		virtual int GetBitsPerPixel()const noexcept = 0;
		virtual void* ReadAllPixels(TextureDataFormat data) = 0;
		//Gets the native handle (uint for opengl).
		virtual void* GetNativeID()noexcept = 0;
		//Generates Mipmaps for the texture.
		virtual void GenerateMipmaps() = 0;

		//Gets the compressed texture data.
		//Will get obsoleted.
		[[deprecated("Will be moved outside of texture.")]] virtual DataBuffer GetCompressed() = 0;

		//Compresses the texture into a new texture object (if possible)
		//Will get obsoleted.
		[[deprecated("Will be moved outside of texture.")]] virtual Texture* Compress() = 0;

		virtual void CopyInto(Texture* Tex, int Height, int Width, int Depth = 0, int SrcMipLevel = 0, int SrcX = 0, int SrcY = 0, int SrcZ = 0, int DstMipLevel = 0, int DstX = 0, int DstY = 0, int DstZ = 0) = 0;
		friend class IRenderer;
	};
}