#include "pch.h"
#include "GLTexture.h"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Sphynx::Core::Graphics;

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
	case TextureFormat::DepthComponent:	return 1;
	case TextureFormat::DepthStencil:	return 2;
	case TextureFormat::Red:			return 1;
	case TextureFormat::RG:				return 2;
	case TextureFormat::RGB:			return 3;
	case TextureFormat::BGR:			return 3;
	case TextureFormat::BGRA:			return 4;
	case TextureFormat::RGBA:			return 4;
	default:
		return 4;
	}
}

inline GLenum GetGLInternalFormat(Sphynx::Core::Graphics::TextureFormat format) {
	//Ehhhhh...
	switch (format)
	{
	case TextureFormat::DepthComponent:				return GL_DEPTH_COMPONENT;
	case TextureFormat::DepthStencil:				return GL_DEPTH_STENCIL;
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
	case TextureFormat::DepthComponent:					return GL_DEPTH_COMPONENT;
	case TextureFormat::DepthStencil:					return GL_DEPTH_STENCIL;
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

void Sphynx::Core::Graphics::GL::GLTexture::Release()
{
	glDeleteTextures(1, &TextureID);
	TextureID = 0;
}

Sphynx::Core::Graphics::GL::GLTexture::GLTexture(const char* path, TextureType Type, int MipmapLevel, TextureFormat format, TextureWrappingMode warp, TextureFilterMode filter, TextureMipmapMode MipmapMode)
{
	GLTextureType = GetGLTextureType(Type);
	int height, width, bits = 0;
	//TODO: Implement Image Loading.
	auto img = stbi_load(path, &width, &height, &bits, 0);
	//TODO: Use the bits variable to determine the Texture Format.
	//TODO: OpenGL is returning an error somewhere before this.
	glCreateTextures(GLTextureType, 1, &TextureID);
	glBindTexture(GLTextureType, TextureID);
	if (Type == TextureType::Texture2D) {
		glTexImage2D(GL_TEXTURE_2D, MipmapLevel, GetGLInternalFormat(format), width, height, 0, GetGLFormat(format), GL_UNSIGNED_BYTE, img);
	}
	else if (Type == TextureType::Texture3D) {
	}
	else if (Type == TextureType::Rectangle) {
		glTexImage2D(GL_TEXTURE_RECTANGLE, MipmapLevel, GetGLInternalFormat(format), width, height, 0,
			GetGLFormat(format), GetGLTextureType(Type), img);
	}//CubeMap
	else {
	}
	glGenerateMipmap(GLTextureType);
	stbi_image_free(img);
	//OpenTextures.insert({ path,this });
}

Sphynx::Core::Graphics::GL::GLTexture::~GLTexture()
{
	Release();
}

Sphynx::Core::Graphics::GL::GLTexture::GLTexture(GLTexture&& tex)noexcept
{
	std::swap(GLTextureType, tex.GLTextureType);
	std::swap(TextureID, tex.TextureID);
	std::swap(DeleteFlag, tex.DeleteFlag);
	std::swap(Refs, tex.Refs);
}

Sphynx::Core::Graphics::GL::GLTexture& Sphynx::Core::Graphics::GL::GLTexture::operator=(GLTexture&& tex)
{
	if (this != &tex) {
		Release();
		std::swap(TextureID, tex.TextureID);
		std::swap(DeleteFlag, tex.DeleteFlag);
		std::swap(Refs, tex.Refs);
	}
	return *this;
}

void Sphynx::Core::Graphics::GL::GLTexture::SetData(const unsigned char* data)
{
}

void Sphynx::Core::Graphics::GL::GLTexture::Bind()
{
	glBindTexture(GLTextureType, TextureID);
}

void Sphynx::Core::Graphics::GL::GLTexture::Unbind()
{
	glBindTexture(GLTextureType, 0);
}
