#pragma once
#include "Core/Graphics/Pipeline/Mesh.h"
namespace Sphynx::Core::Graphics::GL {
	class GLMesh : public Mesh
	{
	public:
		virtual void Create(float* vertexes, size_t vertsize, int* indexes, size_t indexsize, MeshType meshtype) override;
		virtual void Bind() override;
		virtual void UnBind() override;
		inline virtual bool IsValid() override { return VertId; };
		inline bool HasIndexArray(){ return hasIndexArray; };
		inline int GetVertexArraySize() {return VertexSize;};
		inline int GetVertexElementSize() { return (VertexSize / sizeof(float))/3; };
		inline int GetIndexArraySize() { return IndexSize; };
	private:
		unsigned int VertId = 0;
		unsigned int IndexId = 0;
		unsigned int VAO = 0;
		bool hasIndexArray = false;
		int VertexSize;
		int IndexSize;
	};
}
