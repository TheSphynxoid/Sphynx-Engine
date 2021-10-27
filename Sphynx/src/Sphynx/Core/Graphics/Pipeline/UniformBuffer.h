#pragma once

namespace Sphynx::Core::Graphics {
	//TODO: Support For Batching
	enum class BufferAccess {
		Read, Write, ReadWrite
	};

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual void SetData(void* data, size_t size) = 0;
		//If The size is larger than the preallocated buffer this will be ignored.
		virtual void SetData(void* data, size_t offset, size_t size) = 0;
		virtual void Bind(unsigned int BindingPoint) = 0;
		virtual void BindRange(unsigned int BindingPoint, size_t start, size_t end) = 0;
		virtual size_t GetSize() = 0;
		virtual void* MapBuffer(BufferAccess access) = 0;
		virtual void UnMapBuffer() = 0;
		virtual void* GetMappedAddress() = 0;
		static UniformBuffer* Create(size_t size);
		static UniformBuffer* Create(void* Data, size_t size);
	};
}