#pragma once
#include "../Pipeline/UniformBuffer.h"

namespace Sphynx::Core::Graphics::GL {
	//TODO: Support For Batching
	class GLUniformBuffer : public UniformBuffer
	{
	private:
		unsigned int Buffer = 0;
		size_t Size = 0;
		void* MappedAddress = nullptr;
		void Release();
	public:
		GLUniformBuffer(size_t size);
		GLUniformBuffer(void* Data, size_t size);
		GLUniformBuffer(GLUniformBuffer&) = delete;
		GLUniformBuffer& operator=(const GLUniformBuffer&) = delete;
		GLUniformBuffer(GLUniformBuffer&& Ubuf)noexcept;
		GLUniformBuffer& operator=(GLUniformBuffer&& Ubuf)noexcept;
		virtual ~GLUniformBuffer() { Release(); };
		virtual void SetData(void* data, size_t size);
		virtual void SetData(void* data, size_t offset, size_t size);
		virtual void Bind(unsigned int BindingPoint);
		virtual void BindRange(unsigned int BindingPoint ,size_t start, size_t size);
		virtual void* MapBuffer(BufferAccess access);
		virtual void UnMapBuffer();
		virtual void* GetMappedAddress() { return MappedAddress; };
		virtual size_t GetSize() { return Size; };

		friend class GLMaterial;
	};
}