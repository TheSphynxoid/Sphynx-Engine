#pragma once
#include "Core/Graphics/Pipeline/Mesh.h"

namespace Sphynx::Core::Graphics::GL {
	class GLVertexBuffer final : public VertexBuffer {
	private:
		unsigned int BufferID = 0;
		BufferLayout Layout = BufferLayout();
		size_t Size = 0;
		void Release();
	public:
		GLVertexBuffer(size_t size);
		GLVertexBuffer(float* verts, size_t size);
		GLVertexBuffer(const GLVertexBuffer& vbuf) = delete;
		GLVertexBuffer& operator=(const GLVertexBuffer& vbuf) = delete;
		GLVertexBuffer(GLVertexBuffer&& vbuf)noexcept;
		GLVertexBuffer& operator=(GLVertexBuffer&& vbuf)noexcept;
		virtual ~GLVertexBuffer()override { Release(); };
		virtual void Bind()const override;
		virtual void Unbind()const override;
		//Sets the Buffer's Data.
		//In Case size is larger then the Original Size, a new Buffer Will allocated and Offset Will Be Ignored.
		virtual void SetData(const void* data, size_t offset, size_t size)noexcept override;
		virtual void SetDataLayout(BufferLayout layout) override { Layout = layout; };
		virtual size_t GetVertexBufferSize()const noexcept override { return Size; };
		virtual BufferLayout GetLayout()const noexcept override{ return Layout; };
		friend class GLMesh;
	};
	class GLIndexBuffer final : public IndexBuffer {
	private:
		unsigned int BufferID = 0;
		uint32_t Count = 0;
		void Release();
	public:
		GLIndexBuffer(uint32_t count)noexcept;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
		GLIndexBuffer(uint64_t* indices, uint32_t count)noexcept;
=======
		GLIndexBuffer(unsigned int* indices, size_t count)noexcept;
>>>>>>> Stashed changes
=======
		GLIndexBuffer(unsigned int* indices, size_t count)noexcept;
>>>>>>> Stashed changes
=======
		GLIndexBuffer(unsigned int* indices, size_t count)noexcept;
>>>>>>> Stashed changes
		GLIndexBuffer(const GLIndexBuffer& ibuf) = delete;
		GLIndexBuffer& operator=(const GLIndexBuffer& ibuf) = delete;
		GLIndexBuffer(GLIndexBuffer&& ibuf)noexcept;
		GLIndexBuffer& operator=(GLIndexBuffer&& ibuf)noexcept;
		virtual ~GLIndexBuffer()override;
		virtual void Bind()const override;
		virtual void Unbind()const override;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
		virtual void SetData(const uint64_t* data, uint32_t count)override;
=======
		virtual void SetData(const unsigned int* data, uint64_t count)override;
>>>>>>> Stashed changes
=======
		virtual void SetData(const unsigned int* data, uint64_t count)override;
>>>>>>> Stashed changes
=======
		virtual void SetData(const unsigned int* data, uint64_t count)override;
>>>>>>> Stashed changes
		virtual int GetCount()const noexcept override { return Count; };

		friend class GLMesh;
	};
	class GLMesh final : public Mesh
	{
	public:
		//Empty Mesh
		GLMesh();
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
		GLMesh(float* vertexes, size_t vertsize, uint64_t* indexes, size_t indexsize, MeshType meshtype);
		GLMesh(std::vector<VertexBuffer*> VertBuf, IndexBuffer* IndexBuf);
		GLMesh(const GLMesh&) = delete;
		GLMesh& operator=(const GLMesh&) = delete;
		GLMesh(GLMesh&& mesh);
		GLMesh& operator=(GLMesh&& mesh);
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
		GLMesh(float* vertexes, size_t vertsize, unsigned int* indexes, size_t indexsize, MeshType meshtype);
		GLMesh(VertexBuffer* VertBuf, IndexBuffer* IndexBuf);
		GLMesh(std::vector<VertexBuffer*> VertBuf, IndexBuffer* IndexBuf);
		GLMesh(const GLMesh&) = delete;
		GLMesh& operator=(const GLMesh&) = delete;
		GLMesh(GLMesh&& Mesh)noexcept;
		GLMesh& operator=(GLMesh&& Mesh)noexcept;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
		virtual ~GLMesh()override;
		virtual void Bind()const override;
		virtual void UnBind()const override;
		virtual void AddVertexBuffer(VertexBuffer* VBuffer)override;
		virtual void AddVertexBuffers(std::vector<VertexBuffer*> _VBuffers)override;
		virtual void SetIndexBuffer(IndexBuffer* ibuf)override;
		virtual IndexBuffer* GetIndexBuffer()const noexcept override { return IBuffer; };
		virtual size_t GetIndexBufferSize()const noexcept override { return IBuffer->Count * sizeof(uint64_t); };
		virtual std::vector<VertexBuffer*> GetVertexBuffer()const noexcept override { return VBuffers; };
		inline bool HasIndexArray()const noexcept{ return hasIndexArray; };
	private:
		void Release();
		std::vector<VertexBuffer*> VBuffers = std::vector<VertexBuffer*>();
		GLIndexBuffer* IBuffer = nullptr;
		unsigned int VAO = 0;
		bool hasIndexArray = false;
		unsigned int VAttribIndex = 0;
		static int MaxAttribs;
	};
}
