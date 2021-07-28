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
	glCreateVertexArrays(1, &VAO);
	VertexSize = vertsize;
	glCreateBuffers(1, &VertId);
	if (VertId <= 0) {
		Core_Error("Cannot Create Vertex Buffer");
		return;
	}
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VertId);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, vertexes, MeshTypeToGLenum(meshtype));
	if (indexes != nullptr) {
		IndexSize = indexsize;
		hasIndexArray = true;
		glCreateBuffers(1, &IndexId);
		if (IndexId <= 0) {
			Core_Error("Cannot Create Index Buffer");
			return;
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexsize, indexes, MeshTypeToGLenum(meshtype));

		//Attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}

void Sphynx::Core::Graphics::GL::GLMesh::Bind()
{
	glBindVertexArray(VAO);
}

void Sphynx::Core::Graphics::GL::GLMesh::UnBind()
{
	glBindVertexArray(0);
}
