#include "pch.h"
#include "Shader.h"
#include "Core/Graphics/Platform/GLShader.h"

Sphynx::Core::Graphics::Shader* Sphynx::Core::Graphics::Shader::Create(std::string path, ShaderType type)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GL::GLShader(path, type);
#endif
	case Sphynx::Platform::Linux:
		return new GL::GLShader(path, type);
	default:
		break;
	}
	return nullptr;
}

Sphynx::Core::Graphics::Shader* Sphynx::Core::Graphics::Shader::Create(const char* code, ShaderType type)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GL::GLShader(code, type);
#endif
	case Sphynx::Platform::Linux:
		return new GL::GLShader(code, type);
	default:
		break;
	}
	return nullptr;
}
