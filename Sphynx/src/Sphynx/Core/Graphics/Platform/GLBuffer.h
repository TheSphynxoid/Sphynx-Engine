#pragma once
#include "../Pipeline/Buffer.h"

namespace Sphynx::Core::Graphics::GL {
	//This Class is tied with every other buffer type.
	//GLVertexBuffer, GLIndexBuffer have a GLBuffer member.
	class GLBuffer : public GPUBuffer
	{
	private:
		unsigned int id = 0;
		size_t Size = 0;
		unsigned int Usagehint = 0;
	public:
		//Does nothing.
		GLBuffer() = default;
		GLBuffer(size_t size, void* data, const UsageHint hint, const AccessHint accesshint);
		~GLBuffer();
		// Inherited via GPUBuffer
		void Bind()const noexcept override;
		void Unbind()const noexcept override;
		void* Map(const MapAccess access) override;
		void Unmap() noexcept override;
		size_t GetSize() const noexcept override {
			return Size;
		};
		void Reallocate(size_t size, void* data) override;
		void Invalidate() noexcept override;
		void SetData(const void* data, size_t size, size_t offset) override;
		void Release() override;
		//void* GetNative()noexcept override { return &id; };
		void* GetNative()const noexcept override { return (void*)&id; };

		friend class GLRenderer;
	};
}

