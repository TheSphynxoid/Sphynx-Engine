#pragma once

namespace Sphynx::Core::Graphics {
	enum class MapAccess : unsigned char {
		Read, Write, ReadWrite
	};
	enum class UsageHint : unsigned char {
		Stream, Static, Dynamic
	};
	enum class AccessHint : unsigned char {
		Draw, Read, Copy
	};

	struct GPUBuffer {
	public:
		//Create Buffer
		static GPUBuffer* Create(size_t size, void* data, const UsageHint hint, const AccessHint accesshint);
		virtual void Bind()const noexcept = 0;
		virtual void Unbind()const noexcept = 0;
		virtual void* Map(const MapAccess access) = 0;
		virtual void Unmap() noexcept = 0;
		//Get Buffer size.
		virtual size_t GetSize()const noexcept = 0;
		//Reallocate store.
		//For Buffer orphaning.
		virtual void Reallocate(size_t size, void* data) = 0;
		virtual void Invalidate() noexcept = 0;
		virtual void SetData(const void* data, size_t size, size_t offset) = 0;
		virtual void Release() = 0;
		//virtual void* GetNative() noexcept = 0;
		virtual void* GetNative()const noexcept = 0;
	};
}