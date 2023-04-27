#pragma once

namespace Sphynx::Core::Graphics {
	enum class MapAccess {
		Read, Write, ReadWrite
	};

	struct Buffer {
	public:
		virtual void* Map(const MapAccess& access) = 0;
		virtual void Unmap() noexcept = 0;
	};
	//Not Implemented.
	struct GPUBuffer : public Buffer {
	public:
		virtual void* GetLocation() = 0;
		virtual bool IsMapped() = 0;
		virtual size_t GetSize() = 0;
		//Reallocate store.
		virtual void SetSize() = 0;

	};
}