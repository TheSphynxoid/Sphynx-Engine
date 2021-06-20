#include "pch.h"
#include "GLMesh.h"
#include "glad/glad.h"
// #include "GLFW/glfw3.h"

GLenum MeshTypeToGLenum(Sphynx::Core::Graphics::MeshType meshtype) {
	switch (meshtype)
	{
	case Sphynx::Core::Graphics::MeshType::Dynamic:
		return GL_DYNAMIC_DRAW;
	case Sphynx::Core::Graphics::MeshType::Static:
		return GL_STATIC_DRAW;
	case Sphynx::Core::Graphics::MeshType::Stream:
		return GL_STREAM_DRAW;
	default:
		return GL_NONE;
	}
}

void Sphynx::Core::Graphics::GL::GLMesh::Create(float* vertexes, size_t vertsize, int* indexes, size_t indexsize, MeshType meshtype)
{
	VertexSize = vertsize/3;
	glCreateBuffers(1, &VertId);
	if (VertId < 0) {
		Core_Error("Cannot Create Vertex Buffer");
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VertId);
	glBufferData(GL_ARRAY_BUFFER, vertsize, vertexes, MeshTypeToGLenum(meshtype));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (indexes != NULL) {
		IndexSize = indexsize;
		hasIndexArray = true;
		glCreateBuffers(1, &IndexId);
		if (IndexId < 0) {
			Core_Error("Cannot Create Index Buffer");
			return;
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexsize, indexes, MeshTypeToGLenum(meshtype));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Sphynx::Core::Graphics::GL::GLMesh::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, VertId);
	if(hasIndexArray){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexId);
	}
}

void Sphynx::Core::Graphics::GL::GLMesh::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
