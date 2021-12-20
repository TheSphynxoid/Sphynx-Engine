#pragma once
#include "Core/Graphics/Pipeline/FrameBuffer.h"


namespace Sphynx::Core::Graphics {
	class Texture;
}
namespace Sphynx::Core::Graphics::GL {
	class GLTexture;
	class GLFrameBuffer : public FrameBuffer
	{
	private:
		unsigned int ID = 0;
		int Width, Height = 0;
		std::vector<Texture*> Attachements;
		int ColorAttachments = 0;
		void Release();
	public:
		GLFrameBuffer() = delete;
		GLFrameBuffer& operator=(const GLFrameBuffer&) = delete;
		GLFrameBuffer(int width, int height, std::initializer_list<Texture*> attachments);
		virtual void Bind(FrameBufferBinding b = FrameBufferBinding::ReadWrite) override;
		virtual void Unbind() override;
		virtual void Resize(unsigned int width, unsigned int height) override;
		virtual Texture* GetAttachment(size_t index) { return Attachements[index]; };
		virtual void Invalidate() override;
		virtual ~GLFrameBuffer();
	};
}