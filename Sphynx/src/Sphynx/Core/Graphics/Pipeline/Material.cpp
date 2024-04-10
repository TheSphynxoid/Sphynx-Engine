#include "pch.h"
#include "Material.h"
#include "Core/Graphics/Platform/GLMaterial.h"

Sphynx::Core::Graphics::Material* Sphynx::Core::Graphics::Material::Create(const ShaderPack& shaders)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GL::GLMaterial(shaders);
#endif
	case Sphynx::Platform::Linux:
		return new GL::GLMaterial(shaders);
	default:
		break;
	}
	return nullptr;
}

Sphynx::Core::Graphics::Material* Sphynx::Core::Graphics::Material::Create(const ShaderPack& shaders, Texture* texture)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GL::GLMaterial(shaders, texture);
#endif
	case Sphynx::Platform::Linux:
		return new GL::GLMaterial(shaders, texture);
	default:
		break;
	}
    return nullptr;
}

Sphynx::Core::Graphics::Material* Sphynx::Core::Graphics::Material::Create(const ShaderPack& shaders, std::initializer_list<Texture*> textures)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GL::GLMaterial(shaders, textures);
#endif
	case Sphynx::Platform::Linux:
		return new GL::GLMaterial(shaders, textures);
	default:
		break;
	}
	return nullptr;
}

Sphynx::Core::Graphics::Material* Sphynx::Core::Graphics::Material::GetDefaultMaterial()
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return GL::GLMaterial::GetDefaultMaterial();
#endif
	case Sphynx::Platform::Linux:
		return GL::GLMaterial::GetDefaultMaterial();
	default:
		break;
	}
	return nullptr;
}

Sphynx::Core::Graphics::Material* Sphynx::Core::Graphics::Material::GetDefaultMaterialCopy()
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return GL::GLMaterial::CreateDefaultMaterialCopy();
#endif
	case Sphynx::Platform::Linux:
		return GL::GLMaterial::CreateDefaultMaterialCopy();
	default:
		break;
	}
	return nullptr;
}

Sphynx::Core::Graphics::Shader* Sphynx::Core::Graphics::Material::GetDefaultShader(ShaderType type)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return GL::GLMaterial::GetDefaultShader(type);
#endif
	case Sphynx::Platform::Linux:
		return GL::GLMaterial::GetDefaultShader(type);
	default:
		break;
	}
	return nullptr;
}

Sphynx::Core::Graphics::Uniform* Sphynx::Core::Graphics::Uniform::Create(void* data, ShaderDataType type)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
		return new GL::GLUniform(data, type);
	case Sphynx::Platform::Linux:
		return new GL::GLUniform(data, type);
	default:
		break;
	}
	return nullptr;
}
