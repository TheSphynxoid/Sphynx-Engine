#include "pch.h"
#include "FrameBuffer.h"
#include "../Platform/GLFrameBuffer.h"

Sphynx::Core::Graphics::FrameBuffer* Sphynx::Core::Graphics::FrameBuffer::Create(int width, int height, std::initializer_list<Texture*> tex)
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
		return new GL::GLFrameBuffer(width, height, tex);
	default:
		break;
	}
	return nullptr;
    return nullptr;
}
