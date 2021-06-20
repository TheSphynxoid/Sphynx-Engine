#pragma once
#include "Core/Graphics/Pipeline/Mesh.h"
namespace Sphynx::Core::Graphics::GL {
	class GLMesh : public Mesh
	{
	// Inherited via Mesh
	virtual void Create(float* vertexes, size_t vertsize, int* indexes, size_t indexsize, MeshType meshtype) override;
	virtual void Bind() override;
	virtual void UnBind() override;
	public:
		inline bool HasIndexArray(){ return hasIndexArray; };
		inline int GetVertexArraySize() {return VertexSize;};
		inline int GetIndexArraySize() { return IndexSize; };
	private:
		unsigned int VertId = -1;
		unsigned int IndexId = -1;
		bool hasIndexArray = false;
		int VertexSize;
		int IndexSize;
	};
}
