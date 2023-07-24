#include "pch.h"
#include "Texture.h"
#include "Core/Graphics/Platform/GLTexture.h"
#include "ResourceManager.h"

using namespace Sphynx::Core::Graphics;

static TextureFormat DefFormat = TextureFormat::RGB;
static TextureWrappingMode DefWrap = TextureWrappingMode::ClampToBorder;
static TextureFilterMode DefFilter = TextureFilterMode::Linear;
static TextureMipmapMode DefMipmap = TextureMipmapMode::LinearMipmapLinear;

Sphynx::Core::Graphics::Texture::Texture()
{
	Refs++;
}

void Sphynx::Core::Graphics::Texture::SetDefaultFormat(TextureFormat format)
{
	DefFormat = format;
}

void Sphynx::Core::Graphics::Texture::SetDefaultWrappingMode(TextureWrappingMode wrapmode)
{
	DefWrap = wrapmode;
}

void Sphynx::Core::Graphics::Texture::SetDefaultFilterMode(TextureFilterMode filter)
{
	DefFilter = filter;
}

void Sphynx::Core::Graphics::Texture::SetDefaultMipmapMode(TextureMipmapMode mipmapMode)
{
	DefMipmap = mipmapMode;
}

Texture* Sphynx::Core::Graphics::Texture::Create(void* data, int width, int height, int depth, TextureType Type, TextureDataFormat datatype)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		[[fallthrough]];
#endif
	case Sphynx::Platform::Linux:
		return new GL::GLTexture(data, width, height, depth, Type, 0, DefFormat, datatype, DefWrap, DefFilter, DefMipmap);
	default:
		break;
	}
	return nullptr;
}

Texture* Sphynx::Core::Graphics::Texture::Create(void* data, int width, int height, int depth, TextureType Type, TextureFormat format, TextureDataFormat datatype)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		[[fallthrough]];
#endif
	case Sphynx::Platform::Linux:
		return new GL::GLTexture(data, width, height, depth, Type, 0, format, datatype, DefWrap, DefFilter, DefMipmap);
	default:
		break;
	}
	return nullptr;
}

Texture* Sphynx::Core::Graphics::Texture::Create(TextureType Type, int Width, int Height, int depth, TextureFormat format, TextureDataFormat datatype)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		[[fallthrough]];
#endif
	case Sphynx::Platform::Linux:
		return new GL::GLTexture(Type, Width, Height, depth, 0, format, datatype, DefWrap, DefFilter, DefMipmap);
	default:
		break;
	}
	return nullptr;
}

Texture* Sphynx::Core::Graphics::Texture::Create(void* data, int width, int height, int depth, TextureType Type, int MipmapLevel, TextureFormat format, TextureDataFormat datatype, TextureWrappingMode warp, TextureFilterMode filter, TextureMipmapMode MipmapMode)
{
	//if (OpenTextures[path] != nullptr)return OpenTextures[path];
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		[[fallthrough]];
#endif
	case Sphynx::Platform::Linux:
		return new GL::GLTexture(data, width, height, depth, Type, MipmapLevel, format, datatype, warp, filter, MipmapMode);
	default:
		break;
	}
	return nullptr;
}
