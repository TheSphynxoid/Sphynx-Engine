#pragma once
#include "Core/Graphics/Pipeline/Mesh.h"
#include "GLBuffer.h"

namespace Sphynx::Core::Graphics::GL {
	class GLVertexBuffer final : public VertexBuffer {
	private:
		GLBuffer underlaying;
		BufferLayout Layout = BufferLayout();
	public:
		GLVertexBuffer(size_t Size);
		GLVertexBuffer(float* verts, size_t Size);
		GLVertexBuffer(const GLVertexBuffer& vbuf) = delete;
		GLVertexBuffer& operator=(const GLVertexBuffer& vbuf) = delete;
		GLVertexBuffer(GLVertexBuffer&& vbuf)noexcept;
		GLVertexBuffer& operator=(GLVertexBuffer&& vbuf)noexcept;
		virtual ~GLVertexBuffer()override { Release(); };
		virtual void Bind()const noexcept override;
		virtual void Unbind()const noexcept override;
		virtual void SetData(const void* data, size_t size, size_t offset)noexcept override;
		virtual void SetDataLayout(BufferLayout layout) override { Layout = layout; };
		virtual BufferLayout GetLayout()const noexcept override{ return Layout; };
		virtual void Release() override;

		//Buffer

		virtual void* Map(const MapAccess access) override;
		virtual void Unmap()noexcept override;
		friend class GLMesh;

		// Inherited via VertexBuffer
		size_t GetSize() const noexcept override {
			return underlaying.GetSize();
		};
		void Reallocate(size_t size, void* data) override;
		void Invalidate() noexcept override;
		void* GetNative()const noexcept override { return underlaying.GetNative(); };
	};
	class GLIndexBuffer final : public IndexBuffer {
	private:
		GLBuffer underlaying;
	public:
		GLIndexBuffer(uint32_t count)noexcept;
		GLIndexBuffer(unsigned int* indices, size_t count)noexcept;
		GLIndexBuffer(const GLIndexBuffer& ibuf) = delete;
		GLIndexBuffer& operator=(const GLIndexBuffer& ibuf) = delete;
		GLIndexBuffer(GLIndexBuffer&& ibuf)noexcept;
		GLIndexBuffer& operator=(GLIndexBuffer&& ibuf)noexcept;
		virtual ~GLIndexBuffer()override;
		virtual void Bind()const noexcept override;
		virtual void Unbind()const noexcept override;
		virtual void SetData(const void* data, size_t size, size_t offset)override;
		virtual void SetData(const unsigned int* data, uint64_t count)override;
		virtual size_t GetSize()const noexcept override { return underlaying.GetSize(); }
		virtual unsigned int GetCount()const noexcept override { return underlaying.GetSize() / sizeof(int); };
		virtual void* Map(const MapAccess access) override;
		virtual void Unmap()noexcept override;
		virtual void Invalidate()noexcept override;
		virtual void Release() override;
		virtual void Reallocate(size_t size, void* data) override;
		virtual void* GetNative()const noexcept override { return underlaying.GetNative(); };
		friend class GLMesh;
	};
	class GLMesh final : public Mesh
	{
	public:
		//Empty Mesh
		GLMesh()noexcept;
		GLMesh(float* vertexes, size_t vertsize, unsigned int* indexes, size_t indexsize, MeshType meshtype)noexcept;
		GLMesh(VertexBuffer* VertBuf, IndexBuffer* IndexBuf)noexcept;
		GLMesh(std::vector<VertexBuffer*> VertBuf, IndexBuffer* IndexBuf)noexcept;
		GLMesh(const GLMesh&) = delete;
		GLMesh& operator=(const GLMesh&) = delete;
		GLMesh(GLMesh&& Mesh)noexcept;
		GLMesh& operator=(GLMesh&& Mesh)noexcept;
		virtual ~GLMesh()override;
		virtual void Bind()const noexcept override;
		virtual void Unbind()const noexcept override;
		virtual void AddVertexBuffer(VertexBuffer* VBuffer)noexcept override;
		virtual void SetVertexBuffers(std::vector<VertexBuffer*> _VBuffers)noexcept override;
		virtual void SetIndexBuffer(IndexBuffer* ibuf)noexcept override;
		virtual IndexBuffer* GetIndexBuffer()const noexcept override { return IBuffer; };
		virtual std::vector<VertexBuffer*> GetVertexBuffer()const noexcept override { return VBuffers; };
		inline bool HasIndexArray()const noexcept{ return IBuffer; };
		virtual void SetRenderMode(RenderMode m)noexcept { mode = m; };
		virtual RenderMode GetRenderMode()const noexcept { return mode; }
	private:
		void Release()noexcept;
		std::vector<VertexBuffer*> VBuffers = std::vector<VertexBuffer*>();
		GLIndexBuffer* IBuffer = nullptr;
		unsigned int VAO = 0;
		unsigned int VAttribIndex = 0;
		RenderMode mode = RenderMode::Trig;
		static int MaxAttribs;
	};
}
