#include "pch.h"
#include "Mesh.h"
#include "Core/Graphics/Platform/GLMesh.h"

using namespace Sphynx::Core::Graphics::GL;
using namespace Sphynx::Core::Graphics;


VertexBuffer* Sphynx::Core::Graphics::VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLVertexBuffer(vertices, size);
#endif
	case Sphynx::Platform::Linux:
		return new GLVertexBuffer(vertices, size);
	default:
		break;
	}
    return nullptr;
}

IndexBuffer* Sphynx::Core::Graphics::IndexBuffer::Create(uint64_t* indices, size_t size)
{
	switch (CurrentPlatform)
	{
	case Sphynx::Platform::Windows:
#ifdef DX_IMPL
		static_assert(true, "DirectX Not Implemented");
#else
		return new GLIndexBuffer(indices, size);
#endif
	case Sphynx::Platform::Linux:
		return new GLIndexBuffer(indices, size);
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

Mesh* Sphynx::Core::Graphics::Mesh::Create(float* vertexes, size_t vertsize, uint64_t* indexes, size_t indexsize, MeshType meshtype)
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

Mesh* Sphynx::Core::Graphics::Mesh::Create(std::vector<VertexBuffer*> VBuffers, IndexBuffer* IBuffer)
{
	return nullptr;
}
