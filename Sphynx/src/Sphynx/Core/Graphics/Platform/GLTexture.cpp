#include "pch.h"
#include "GLTexture.h"
#include "glad/glad.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

using namespace Sphynx::Core::Graphics;

//Redefined. Does the compiler optimize this ?

inline GLenum GetGLTextureType(Sphynx::Core::Graphics::TextureType type) {
	switch (type)
	{
	case TextureType::Texture2D:	return GL_TEXTURE_2D;
	case TextureType::Texture3D:	return GL_TEXTURE_3D;
	case TextureType::CubeMap:		return GL_TEXTURE_CUBE_MAP;
	case TextureType::Rectangle:	return GL_TEXTURE_RECTANGLE;
	default:
		Core_Error("Unknown Texture Type");
		return 0;
	}
}

inline int GetNBitsFromGLFormat(Sphynx::Core::Graphics::TextureFormat format) {
	switch (format)
	{
		//TODO: Complete to GL Internal Formats
	case TextureFormat::Depth24_Stencil8:	return 2;
	case TextureFormat::Red:				return 1;
	case TextureFormat::RG:					return 2;
	case TextureFormat::RGB:				return 3;
	case TextureFormat::BGR:				return 3;
	case TextureFormat::BGRA:				return 4;
	case TextureFormat::RGBA:				return 4;
	default:
		return 4;
	}
}

inline GLenum GetGLDataType(Sphynx::Core::Graphics::TextureDataFormat dformat) {
	switch (dformat)
	{
	case TextureDataFormat::UByte: 					return GL_UNSIGNED_BYTE;
	case TextureDataFormat::Byte:					return GL_BYTE;
	case TextureDataFormat::UShort: 				return GL_UNSIGNED_INT;
	case TextureDataFormat::Short: 					return GL_SHORT;
	case TextureDataFormat::UInt: 					return GL_UNSIGNED_INT;
	case TextureDataFormat::Int: 					return GL_INT;
	case TextureDataFormat::HalfFloat:				return GL_HALF_FLOAT;
	case TextureDataFormat::Float: 					return GL_FLOAT;
	case TextureDataFormat::UByte_3_3_2:			return GL_UNSIGNED_BYTE_3_3_2;
	case TextureDataFormat::UByte_2_3_3_REV: 		return GL_UNSIGNED_BYTE_2_3_3_REV;
	case TextureDataFormat::UShort_5_6_5: 			return GL_UNSIGNED_SHORT_5_6_5;
	case TextureDataFormat::UShort_5_6_5_REV:		return GL_UNSIGNED_SHORT_5_6_5_REV;
	case TextureDataFormat::UShort_4_4_4_4:			return GL_UNSIGNED_SHORT_4_4_4_4;
	case TextureDataFormat::UShort_4_4_4_4_REV: 	return GL_UNSIGNED_SHORT_4_4_4_4_REV;
	case TextureDataFormat::UShort_5_5_5_1: 		return GL_UNSIGNED_SHORT_5_5_5_1;
	case TextureDataFormat::UShort_1_5_5_5_REV: 	return GL_UNSIGNED_SHORT_1_5_5_5_REV;
	case TextureDataFormat::UInt_8_8_8_8: 			return GL_UNSIGNED_INT_8_8_8_8;
	case TextureDataFormat::UInt_8_8_8_8_REV: 		return GL_UNSIGNED_INT_8_8_8_8_REV;
	case TextureDataFormat::UInt_10_10_10_2: 		return GL_UNSIGNED_INT_10_10_10_2;
	case TextureDataFormat::UInt_2_10_10_10_REV: 	return GL_UNSIGNED_INT_2_10_10_10_REV;
	case TextureDataFormat::UInt_24_8:				return GL_UNSIGNED_INT_24_8;
	default:										return GL_UNSIGNED_BYTE;
	}
}

inline GLenum GetGLInternalFormat(Sphynx::Core::Graphics::TextureFormat format) {
	//Ehhhhh...
	switch (format)
	{
	case TextureFormat::Stencil:					return GL_STENCIL_INDEX8;
	case TextureFormat::Depth_Component16:			return GL_DEPTH_COMPONENT16;
	case TextureFormat::Depth_Component24:			return GL_DEPTH_COMPONENT24;
	case TextureFormat::Depth_Component32:			return GL_DEPTH_COMPONENT32;
	case TextureFormat::Depth_Component32F:			return GL_DEPTH_COMPONENT32F;
	case TextureFormat::Depth24_Stencil8:			return GL_DEPTH24_STENCIL8;
	case TextureFormat::Depth32F_Stencil8:			return GL_DEPTH32F_STENCIL8;
	case TextureFormat::Red:						return GL_RED;
	case TextureFormat::Red8:						return GL_R8;
	case TextureFormat::Red8SNorm:					return GL_R8_SNORM;
	case TextureFormat::Red16:						return GL_R16;
	case TextureFormat::Red16SNorm:					return GL_R16_SNORM;
	case TextureFormat::RG:							return GL_RG;
	case TextureFormat::RG8:						return GL_RG8;
	case TextureFormat::RG8SNorm:					return GL_RG8_SNORM;
	case TextureFormat::RG16:						return GL_RG16;
	case TextureFormat::RG16SNorm:					return GL_RG16_SNORM;
	case TextureFormat::R3_G3_B2:					return GL_R3_G3_B2;
	case TextureFormat::RGB565:						return GL_RGB565;
	case TextureFormat::RGB:						return GL_RGB;
	case TextureFormat::RGB4:						return GL_RGB4;
	case TextureFormat::RGB5:						return GL_RGB5;
	case TextureFormat::RGB8:						return GL_RGB8;
	case TextureFormat::RGB8SNorm:					return GL_RGB8_SNORM;
	case TextureFormat::RGB10:						return GL_RGB10;
	case TextureFormat::RGB12:						return GL_RGB12;
	case TextureFormat::RGB16:						return GL_RGB16;
	case TextureFormat::RGB16SNorm:					return GL_RGB16_SNORM;
	case TextureFormat::RGBA:						return GL_RGBA;
	case TextureFormat::RGBA2:						return GL_RGBA2;
	case TextureFormat::RGBA4:						return GL_RGBA4;
	case TextureFormat::RGB5_A1:					return GL_RGB5_A1;
	case TextureFormat::RGBA8:						return GL_RGBA8;
	case TextureFormat::RGBA8SNorm:					return GL_RGBA8_SNORM;
	case TextureFormat::RGB10_A2:					return GL_RGB10_A2;
	case TextureFormat::UIntRGB10_A2:				return GL_RGB10_A2UI;
	case TextureFormat::RGBA12:						return GL_RGBA12;
	case TextureFormat::RGBA16:						return GL_RGBA16;
	case TextureFormat::SRGB8:						return GL_SRGB8;
	case TextureFormat::SRGB8_A8:					return GL_SRGB8_ALPHA8;
	case TextureFormat::BGR:						return GL_RGB;
	case TextureFormat::BGRA:						return GL_RGBA;
	case TextureFormat::FloatR16:					return GL_R16F;
	case TextureFormat::FloatRG16:					return GL_RG16F;
	case TextureFormat::FloatRGB16:					return GL_RGB16F;
	case TextureFormat::FloatRGBA16:				return GL_RGBA16F;
	case TextureFormat::FloatR32:					return GL_R32F;
	case TextureFormat::FloatRG32:					return GL_RG32F;
	case TextureFormat::FloatRGB32:					return GL_RGB32F;
	case TextureFormat::FloatRGBA32:				return GL_RGBA32F;
	case TextureFormat::FloatRG11_B10:				return GL_R11F_G11F_B10F;
	case TextureFormat::RGB9_E5:					return GL_RGB9_E5;
	case TextureFormat::IntRed8:					return GL_R8I;
	case TextureFormat::UIntRed8:					return GL_R8UI;
	case TextureFormat::IntRed16:					return GL_R16I;
	case TextureFormat::UIntRed16:					return GL_R16UI;
	case TextureFormat::IntRed32:					return GL_R32I;
	case TextureFormat::UIntRed32:					return GL_R32UI;
	case TextureFormat::IntRG8:						return GL_RG8I;
	case TextureFormat::UIntRG8:					return GL_RG8UI;
	case TextureFormat::IntRG16:					return GL_RG16I;
	case TextureFormat::UIntRG16:					return GL_RG16UI;
	case TextureFormat::UIntRG32:					return GL_RG32UI;
	case TextureFormat::IntRGB8:					return GL_RGB8I;
	case TextureFormat::UIntRGB8:					return GL_RGB8UI;
	case TextureFormat::IntRGB16:					return GL_RGB16I;
	case TextureFormat::UIntRGB16:					return GL_RGB16UI;
	case TextureFormat::IntRGB32:					return GL_RGB32I;
	case TextureFormat::UIntRGB32:					return GL_RGB32UI;
	case TextureFormat::IntRGBA8:					return GL_RGBA8I;
	case TextureFormat::UIntRGBA8:					return GL_RGBA8UI;
	case TextureFormat::IntRGBA16:					return GL_RGBA16I;
	case TextureFormat::UIntRGBA16:					return GL_RGBA16UI;
	case TextureFormat::IntRGBA32:					return GL_RGBA32I;
	case TextureFormat::UIntRGBA32:					return GL_RGBA32UI;
	case TextureFormat::CompressedRed:				return GL_COMPRESSED_RED;
	case TextureFormat::CompressedRG:				return GL_COMPRESSED_RG;
	case TextureFormat::CompressedRGB:				return GL_COMPRESSED_RGB;
	case TextureFormat::CompressedRGBA:				return GL_COMPRESSED_RGBA;
	case TextureFormat::CompressedSRGB:				return GL_COMPRESSED_SRGB;
	case TextureFormat::CompressedSRGB_A:			return GL_COMPRESSED_SRGB_ALPHA;
	case TextureFormat::CompressedRed_RGTC1:		return GL_COMPRESSED_RED_RGTC1;
	case TextureFormat::SignedCompressedRed_RGTC1:	return GL_COMPRESSED_SIGNED_RED_RGTC1;
	case TextureFormat::CompressedRG_RGTC2:			return GL_COMPRESSED_RG_RGTC2;
	case TextureFormat::CompressedRGBA_BPTC_Unorm:	return GL_COMPRESSED_RGBA_BPTC_UNORM;
	case TextureFormat::CompressedSRGB_A_BPTC_UNorm:return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
	case TextureFormat::FloatCompressedRGB_BPTC:	return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
	case TextureFormat::UFloatCompressedRGB_BPTC:	return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
	default:										return NULL;
	}
}

GLenum GetGLFormat(Sphynx::Core::Graphics::TextureFormat format) {
	//Yeah...
	switch (format)
	{
	case TextureFormat::Stencil:						return GL_STENCIL_INDEX;
	case TextureFormat::Depth_Component16:				return GL_DEPTH_COMPONENT;
	case TextureFormat::Depth_Component24:				return GL_DEPTH_COMPONENT;
	case TextureFormat::Depth_Component32:				return GL_DEPTH_COMPONENT;
	case TextureFormat::Depth_Component32F:				return GL_DEPTH_COMPONENT;
	case TextureFormat::Depth24_Stencil8:				return GL_DEPTH_STENCIL;
	case TextureFormat::Depth32F_Stencil8:				return GL_DEPTH_STENCIL;
	case TextureFormat::Red:							return GL_RED;
	case TextureFormat::Red8:							return GL_RED;
	case TextureFormat::Red8SNorm:						return GL_RED;
	case TextureFormat::Red16:							return GL_RED;
	case TextureFormat::Red16SNorm:						return GL_RED;
	case TextureFormat::RG:								return GL_RG;
	case TextureFormat::RG8:							return GL_RG;
	case TextureFormat::RG8SNorm:						return GL_RG;
	case TextureFormat::RG16:							return GL_RG;
	case TextureFormat::RG16SNorm:						return GL_RG;
	case TextureFormat::R3_G3_B2:						return GL_RGB;
	case TextureFormat::RGB:		 					return GL_RGB;
	case TextureFormat::RGB4:		 					return GL_RGB;
	case TextureFormat::RGB5:		 					return GL_RGB;
	case TextureFormat::RGB8:		 					return GL_RGB;
	case TextureFormat::RGB8SNorm:		 				return GL_RGB;
	case TextureFormat::RGB10:		 					return GL_RGB;
	case TextureFormat::RGB12:		 					return GL_RGB;
	case TextureFormat::RGB16:		 					return GL_RGB;
	case TextureFormat::RGB16SNorm:		 				return GL_RGB;
	case TextureFormat::RGBA:		 					return GL_RGBA;
	case TextureFormat::RGBA2:		 					return GL_RGBA;
	case TextureFormat::RGBA4:		 					return GL_RGBA;
	case TextureFormat::RGB5_A1:		 				return GL_RGBA;
	case TextureFormat::RGBA8:		 					return GL_RGBA;
	case TextureFormat::RGBA8SNorm:		 				return GL_RGBA;
	case TextureFormat::RGB10_A2:		 				return GL_RGBA;
	case TextureFormat::UIntRGB10_A2:		 			return GL_RGBA;
	case TextureFormat::RGBA12:		 					return GL_RGBA;
	case TextureFormat::RGBA16:		 					return GL_RGBA;
	case TextureFormat::SRGB8:		 					return GL_RGB;
	case TextureFormat::SRGB8_A8:		 				return GL_RGBA;
	case TextureFormat::BGR:		 					return GL_BGR;
	case TextureFormat::BGRA:		 					return GL_BGRA;
	case TextureFormat::FloatR16:		 				return GL_RED;
	case TextureFormat::FloatRG16:		 				return GL_RG;
	case TextureFormat::FloatRGB16:		 				return GL_RGB;
	case TextureFormat::FloatRGBA16:		 			return GL_RGBA;
	case TextureFormat::FloatR32:		 				return GL_RED;
	case TextureFormat::FloatRG32:		 				return GL_RG;
	case TextureFormat::FloatRGB32:		 				return GL_RGB;
	case TextureFormat::FloatRGBA32:		 			return GL_RGBA;
	case TextureFormat::FloatRG11_B10:		 			return GL_RGB;
	case TextureFormat::RGB9_E5:		 				return GL_RGB;
	case TextureFormat::IntRed8:		 				return GL_RED;
	case TextureFormat::UIntRed8:		 				return GL_RED;
	case TextureFormat::IntRed16:		 				return GL_RED;
	case TextureFormat::UIntRed16:		 				return GL_RED;
	case TextureFormat::IntRed32:		 				return GL_RED;
	case TextureFormat::UIntRed32:		 				return GL_RED;
	case TextureFormat::IntRG8:							return GL_RG;
	case TextureFormat::UIntRG8:		 				return GL_RG;
	case TextureFormat::IntRG16:		 				return GL_RG;
	case TextureFormat::UIntRG16:		 				return GL_RG;
	case TextureFormat::UIntRG32:		 				return GL_RG;
	case TextureFormat::IntRGB8:						return GL_RGB;
	case TextureFormat::UIntRGB8:						return GL_RGB;
	case TextureFormat::IntRGB16:						return GL_RGB;
	case TextureFormat::UIntRGB16:						return GL_RGB;
	case TextureFormat::IntRGB32:						return GL_RGB;
	case TextureFormat::UIntRGB32:						return GL_RGB;
	case TextureFormat::IntRGBA8:						return GL_RGBA;
	case TextureFormat::UIntRGBA8:						return GL_RGBA;
	case TextureFormat::IntRGBA16:						return GL_RGBA;
	case TextureFormat::UIntRGBA16:						return GL_RGBA;
	case TextureFormat::IntRGBA32:						return GL_RGBA;
	case TextureFormat::UIntRGBA32:						return GL_RGBA;
	case TextureFormat::CompressedRed:					return GL_RED;
	case TextureFormat::CompressedRG:					return GL_RG;
	case TextureFormat::CompressedRGB:					return GL_RGB;
	case TextureFormat::CompressedRGBA:					return GL_RGBA;
	case TextureFormat::CompressedSRGB:					return GL_RGB;
	case TextureFormat::CompressedSRGB_A:				return GL_RGBA;
	case TextureFormat::CompressedRed_RGTC1:			return GL_RED;
	case TextureFormat::SignedCompressedRed_RGTC1:		return GL_RED;
	case TextureFormat::CompressedRG_RGTC2:				return GL_RG;
	case TextureFormat::CompressedRGBA_BPTC_Unorm:		return GL_RGBA;
	case TextureFormat::CompressedSRGB_A_BPTC_UNorm:	return GL_RGBA;
	case TextureFormat::FloatCompressedRGB_BPTC:		return GL_RGB;
	case TextureFormat::UFloatCompressedRGB_BPTC:		return GL_RGB;
	default:											return NULL;
	}
}

GLenum GetGLFilter(Sphynx::Core::Graphics::TextureFilterMode filter) {
	switch (filter)
	{
	case Sphynx::Core::Graphics::TextureFilterMode::Nearest:	return GL_NEAREST;
	case Sphynx::Core::Graphics::TextureFilterMode::Linear:		return GL_LINEAR;
	default:													return GL_LINEAR;
	}
	return 0;
}

void Sphynx::Core::Graphics::GL::GLTexture::Release()
{
	glDeleteTextures(1, &TextureID);
	TextureID = 0;
}

Sphynx::Core::Graphics::GL::GLTexture::GLTexture(void* img, int width, int height, int depth, TextureType type, int MipmapLevel, 
	TextureFormat format, TextureDataFormat datatype, TextureWrappingMode warp, TextureFilterMode filter, TextureMipmapMode MipmapMode)
{
	Type = type;
	GLTextureType = GetGLTextureType(Type);
	Format = format;
	DataFormat = datatype;
	MipMapLevel = MipmapLevel;
	Width = width;
	Height = height;
	Depth = depth;
	//TODO: Use the bits variable to determine the Texture Format.
	glCreateTextures(GLTextureType, 1, &TextureID);
	glBindTexture(GLTextureType, TextureID);
	if (Type == TextureType::Texture2D) {
		/*if (datatype == TextureDataFormat::Byte || datatype == TextureDataFormat::UByte) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}*/
		glTexImage2D(GL_TEXTURE_2D, MipmapLevel, GetGLInternalFormat(format), Width, Height, 0,
			GetGLFormat(format), GetGLDataType(datatype), img);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	else if (Type == TextureType::Texture3D) {
	}
	else if (Type == TextureType::Rectangle) {
		glTexImage2D(GL_TEXTURE_RECTANGLE, MipmapLevel, GetGLInternalFormat(format), Width, Height, 0,
			GetGLFormat(format), GetGLDataType(datatype), img);
	}//CubeMap
	else {
	}
	glTexParameteri(GLTextureType, GL_TEXTURE_MAG_FILTER, GetGLFilter(filter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilter(filter));
}

Sphynx::Core::Graphics::GL::GLTexture::GLTexture(TextureType type, int width, int height, int depth, int MipmapLevel, 
	TextureFormat format, TextureDataFormat datatype, TextureWrappingMode warp, TextureFilterMode filter, TextureMipmapMode MipmapMode)
{
	GLTextureType = GetGLTextureType(type);
	Type = type;
	Format = format;
	Width = width;
	Depth = depth;
	Height = height;
	Bits = GetNBitsFromGLFormat(Format);
	DataFormat = datatype;
	MipMapLevel = MipmapLevel;
	//TODO: Use the bits variable to determine the Texture Format.
	//TODO: OpenGL is returning an error somewhere before this.
	glCreateTextures(GLTextureType, 1, &TextureID);
	glBindTexture(GLTextureType, TextureID);
	//TODO: Implement Image Loading.
	if (Type == TextureType::Texture2D) {
		glTexImage2D(GL_TEXTURE_2D, MipmapLevel, GetGLInternalFormat(format), width, height, 0, GetGLFormat(format), GetGLDataType(datatype), 0);
	}
	else if (Type == TextureType::Texture3D) {
	}
	else if (Type == TextureType::Rectangle) {
		glTexImage2D(GL_TEXTURE_RECTANGLE, MipmapLevel, GetGLInternalFormat(format), width, height, 0,
			GetGLFormat(format), GetGLDataType(datatype), 0);
	}//CubeMap
	else {
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

Sphynx::Core::Graphics::GL::GLTexture::~GLTexture()
{
	Release();
}

Sphynx::Core::Graphics::GL::GLTexture::GLTexture(GLTexture&& tex)noexcept
{
	std::swap(GLTextureType, tex.GLTextureType);
	std::swap(TextureID, tex.TextureID);
	std::swap(Format, tex.Format);
	std::swap(Type, tex.Type);
	std::swap(Width, tex.Width);
	std::swap(Height, tex.Height);
	std::swap(Depth, tex.Depth);
	std::swap(Refs, tex.Refs);
	std::swap(DataFormat, tex.DataFormat);
}

Sphynx::Core::Graphics::GL::GLTexture& Sphynx::Core::Graphics::GL::GLTexture::operator=(GLTexture&& tex)noexcept
{
	if (this != &tex) {
		Release();
		std::swap(GLTextureType, tex.GLTextureType);
		std::swap(TextureID, tex.TextureID);
		std::swap(Format, tex.Format);
		std::swap(Type, tex.Type);
		std::swap(Width, tex.Width);
		std::swap(Height, tex.Height);
		std::swap(Refs, tex.Refs);
	}
	return *this;
}

void Sphynx::Core::Graphics::GL::GLTexture::SetData(void* data, int Level, int OffsetX, int OffsetY, int OffsetZ, int width, int height, int depth)
{
	if (width == -1 || height == -1 || depth == -1) { width = this->Width; height = this->Height; depth = Depth; }
	if (width > this->Width || height > this->Height || depth > this->Depth)Resize(Width, Height, Depth);
	switch (Type)
	{
	case Sphynx::Core::Graphics::TextureType::Texture1D:
		glTextureSubImage1D(TextureID, Level, OffsetX, width, GetGLFormat(Format), GetGLDataType(DataFormat), data);
		break;
	case Sphynx::Core::Graphics::TextureType::Texture1D_Array:
		__fallthrough;
	case Sphynx::Core::Graphics::TextureType::Texture2D:
		glTextureSubImage2D(TextureID, Level, OffsetX, OffsetY, width, height, GetGLFormat(Format), GetGLDataType(DataFormat), data);
		break;
	case Sphynx::Core::Graphics::TextureType::Texture2D_Array:
		__fallthrough;
	case Sphynx::Core::Graphics::TextureType::Texture3D:
		__fallthrough;
	case Sphynx::Core::Graphics::TextureType::CubeMap:
		glTextureSubImage3D(TextureID, Level, OffsetX, OffsetY, OffsetZ, width, height, depth, GetGLFormat(Format), GetGLDataType(DataFormat), data);
		break;
	case Sphynx::Core::Graphics::TextureType::Rectangle:
		glTextureSubImage2D(TextureID, Level, OffsetX, OffsetY, width, height, GetGLFormat(Format), GetGLDataType(DataFormat), data);
		break;
	default:
		break;
	}
}

void Sphynx::Core::Graphics::GL::GLTexture::Resize(int width, int height, int depth, int Level, TextureFormat format, TextureDataFormat dataformat, const void* data)
{
	Bind();
	Width = width; Height = height; Depth = depth;
	switch (Type)
	{
	case Sphynx::Core::Graphics::TextureType::Texture2D:
		glTexImage2D(GL_TEXTURE_2D, Level, GetGLInternalFormat(format), width, height, 0, GetGLFormat(format), GetGLDataType(dataformat), data);
		break;
	case Sphynx::Core::Graphics::TextureType::Texture2D_Array:
		glTexImage3D(GL_TEXTURE_2D_ARRAY, Level, GetGLInternalFormat(format), width, height, depth, 0, GetGLFormat(format), GetGLDataType(dataformat), data);
		break;
	case Sphynx::Core::Graphics::TextureType::Texture3D:
		break;
	case Sphynx::Core::Graphics::TextureType::CubeMap:
		break;
	case Sphynx::Core::Graphics::TextureType::Rectangle:
		break;
	default:
		break;
	}
}

void Sphynx::Core::Graphics::GL::GLTexture::Clear(int level, int OffsetX, int OffsetY, int OffsetZ, int Depth, int Width, int Height, TextureFormat format, TextureDataFormat dataformat, const void* data)
{
	glClearTexSubImage(this->TextureID, level, OffsetX, OffsetY, OffsetZ, Width, Height, Depth, 
		format == (TextureFormat)-1 ? GetGLFormat(Format) : GetGLFormat(format), 
		dataformat == (TextureDataFormat)-1 ? GetGLDataType(DataFormat) : GetGLDataType(dataformat), data);
}

void Sphynx::Core::Graphics::GL::GLTexture::Clear(int Level, TextureFormat format, TextureDataFormat dataformat, const void* data)
{
	glClearTexImage(this->TextureID, Level, GetGLFormat(format), GetGLDataType(dataformat), data);
}

void Sphynx::Core::Graphics::GL::GLTexture::Clear(int Level)
{
	glClearTexImage(TextureID, Level, 0, 0, 0);
}

void Sphynx::Core::Graphics::GL::GLTexture::Bind()noexcept
{
	glBindTexture(GLTextureType, TextureID);
}

void Sphynx::Core::Graphics::GL::GLTexture::Unbind()noexcept
{
	glBindTexture(GLTextureType, 0);
}

void* Sphynx::Core::Graphics::GL::GLTexture::ReadAllPixels(TextureDataFormat data)
{
	//glBindTexture(GL_PIXEL_PACK_BUFFER, TextureID);
	Bind();
	Core_Warn("Texture::ReadAllPixels Is Incomplete : Must Get the number of bits per pixel to allocate buffer");
	void* buf = malloc(sizeof(int) * Width * Height * Bits);
	if (!buf)throw std::bad_alloc();
	memset(buf, 0, sizeof(int) * Width * Height * Bits);
	//TODO: Implement this for other texture formats.
	glReadPixels(0, 0, Width, Height, GetGLFormat(Format), GetGLDataType(data), buf);
	return buf;
}

void Sphynx::Core::Graphics::GL::GLTexture::GenerateMipmaps()
{
	glGenerateTextureMipmap(this->TextureID);
}

DataBuffer Sphynx::Core::Graphics::GL::GLTexture::GetCompressed()
{
	//Check whether the texture is compressed or not.
	int Val;
	glGetTextureLevelParameteriv(this->TextureID, MipMapLevel, GL_TEXTURE_COMPRESSED, &Val);
	if (Val) {
		glGetTextureLevelParameteriv(TextureID, MipMapLevel, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &Val);
		DataBuffer buf = { 0,(size_t)Val };
		glGetCompressedTextureImage(TextureID, MipMapLevel, buf.Size, &buf.Data);
		return buf;
	}
	return { nullptr,0 };
}

Texture* Sphynx::Core::Graphics::GL::GLTexture::Compress()
{
	return nullptr;
}

void Sphynx::Core::Graphics::GL::GLTexture::CopyInto(Texture* Tex, int Height, int Width, int Depth, int SrcMipLevel, int SrcX, int SrcY, int SrcZ, int DstMipLevel, int DstX, int DstY, int DstZ)
{
	glCopyImageSubData(TextureID, GetGLTextureType(Type), SrcMipLevel, SrcX, SrcY, SrcZ, *(GLuint*)Tex->GetNativeID(), GetGLTextureType(Tex->GetTextureType()), DstMipLevel, DstX, DstY, DstZ, Width, Height, Depth);
}
