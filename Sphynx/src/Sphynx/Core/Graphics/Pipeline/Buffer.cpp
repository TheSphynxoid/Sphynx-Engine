#include "pch.h"
#include "Buffer.h"
#ifdef Platform_Windows
#include "../Platform/GLBuffer.h"
#elif Platform_Linux
#include "../Platform/GLBuffer.h"
#endif

Sphynx::Core::Graphics::GPUBuffer* Sphynx::Core::Graphics::GPUBuffer::Create(size_t size, void* data, const UsageHint hint, const AccessHint accesshint)
{
	switch (CurrentPlatform)
	{
	case Platform::Windows:
		[[fallthrough]];
	case Platform::Linux:
		return new GL::GLBuffer(size, data, hint, accesshint);
	}
    return nullptr;
}
