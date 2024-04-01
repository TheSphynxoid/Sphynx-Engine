#include "pch.h"
#include "Mesh.h"
#if defined(Platform_Windows) && defined(DX_IMPL)
#else
#include "Core/Graphics/Platform/GLMesh.h"
using namespace Sphynx::Core::Graphics::GL;
#endif
using namespace Sphynx::Core::Graphics;
VertexBuffer* Sphynx::Core::Graphics::VertexBuffer::Create(float* vertices, size_t Size) noexcept
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLVertexBuffer(vertices, Size);
#endif
	case Sphynx::Platform::Linux:
		return new GLVertexBuffer(vertices, Size);
	default:
		break;
	}
    return nullptr;
}

VertexBuffer* Sphynx::Core::Graphics::VertexBuffer::CreateEmpty(size_t Size) noexcept
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLVertexBuffer(Size);
#endif
	case Sphynx::Platform::Linux:
		return new GLVertexBuffer(Size);
	default:
		break;
	}
	return nullptr;
}

IndexBuffer* Sphynx::Core::Graphics::IndexBuffer::Create(unsigned int* indices, size_t Size) noexcept
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLIndexBuffer(indices, Size);
#endif
	case Sphynx::Platform::Linux:
		return new GLIndexBuffer(indices, Size);
	default:
		break;
	}
	return nullptr;
}

Mesh* Sphynx::Core::Graphics::Mesh::CreateEmpty()
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLMesh();
#endif
	case Sphynx::Platform::Linux:
		return new GLMesh();
	default:
		break;
	}
	return nullptr;
}

Mesh* Sphynx::Core::Graphics::Mesh::Create(float* vertexes, size_t vertsize, unsigned int* indexes, size_t indexsize, MeshType meshtype)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLMesh(vertexes, vertsize, indexes, indexsize, meshtype);
#endif
	case Sphynx::Platform::Linux:
		return new GLMesh(vertexes, vertsize, indexes, indexsize, meshtype);
	default:
		break;
	}
	return nullptr;
}
Mesh* Sphynx::Core::Graphics::Mesh::Create(VertexBuffer* VBuffer, IndexBuffer* IBuffer)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLMesh(VBuffer, IBuffer);
#endif
	case Sphynx::Platform::Linux:
		return new GLMesh(VBuffer, IBuffer);
	default:
		break;
	}
	return nullptr;
}

Mesh* Sphynx::Core::Graphics::Mesh::Create(std::vector<VertexBuffer*> VBuffers, IndexBuffer* IBuffer)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLMesh(VBuffers, IBuffer);
#endif
	case Sphynx::Platform::Linux:
		return new GLMesh(VBuffers, IBuffer);
	default:
		break;
	}
	return nullptr;
}
