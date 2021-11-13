#include "pch.h"
#include "UniformBuffer.h"
#if defined(Platform_Windows) && defined(DX_IMPL)
#else
#include "Core/Graphics/Platform/GLUniformBuffer.h"
using namespace Sphynx::Core::Graphics::GL;
#endif

using namespace Sphynx::Core::Graphics;

Sphynx::Core::Graphics::UniformBuffer* Sphynx::Core::Graphics::UniformBuffer::Create(size_t Size)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLUniformBuffer(Size);
#endif
	case Sphynx::Platform::Linux:
		return new GLUniformBuffer(Size);
	default:
		break;
	}
	return nullptr;
}

Sphynx::Core::Graphics::UniformBuffer* Sphynx::Core::Graphics::UniformBuffer::Create(void* Data, size_t Size)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLUniformBuffer(Data, Size);
#endif
	case Sphynx::Platform::Linux:
		return new GLUniformBuffer(Data, Size);
	default:
		break;
	}
	return nullptr;
}
