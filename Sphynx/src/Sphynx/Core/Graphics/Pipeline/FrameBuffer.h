#pragma once
#include "Events/Event.h"

namespace Sphynx::Core::Graphics {
	class Texture;
	enum class FrameBufferBinding {
		Read, Write, ReadWrite
	};
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		virtual void Bind(FrameBufferBinding b = FrameBufferBinding::ReadWrite) = 0;
		virtual void Unbind() = 0;
		virtual void Resize(unsigned int width, unsigned int height) = 0;
		virtual Texture* GetAttachment(size_t index) = 0;
		virtual void Invalidate() = 0;
		static FrameBuffer* Create(int width, int height, std::initializer_list<Texture*> tex);
	};
}