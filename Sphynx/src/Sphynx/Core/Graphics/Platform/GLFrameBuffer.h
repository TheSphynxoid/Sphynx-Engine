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
		int Width = 0, Height = 0;
		Texture* DepthAttachment = nullptr;
		std::vector<Texture*> ColorAttachments;
		int ColorAttachmentsCount = 0;
		void Release();
		//Used to Create the Default framebuffer
		GLFrameBuffer(unsigned int id, int width, int height, std::initializer_list<Texture*> attachments = {});
	public:
		GLFrameBuffer() = delete;
		GLFrameBuffer& operator=(const GLFrameBuffer&) = delete;
		GLFrameBuffer(int width, int height, std::initializer_list<Texture*> attachments = {});
		virtual void Bind(FrameBufferBinding b = FrameBufferBinding::ReadWrite) override;
		virtual void Unbind() override;
		virtual void Resize(unsigned int width, unsigned int height) override;
		virtual Texture* GetColorAttachment(size_t index) { return ColorAttachments[index]; };
		virtual void Invalidate() override;
		virtual bool IsDefaultFrameBuffer() override { return !ID; };
		virtual int GetWidth() { return Width; };
		virtual int GetHeight() { return Height; };
		virtual ~GLFrameBuffer();
		virtual void AddColorAttachment(Texture* tex) override;
		virtual void SetDepthStencilAttachment(Texture* tex) override;
		virtual bool HasDepthAttachment() override {
			return DepthAttachment;
		};
		virtual Texture* GetDepthStencilAttachment() override {
			return DepthAttachment;
		};
		//Useless.
		virtual bool HasStencilAttachment() override {
			return DepthAttachment;
		};
		virtual bool IsValid() {
			return ColorAttachmentsCount;
		}
		virtual void SetClearColor(glm::vec4 col);
		virtual void Clear(ClearBuffer b);
		virtual void Clear();
		virtual void* GetNativeID() override { return (void*)ID; };
		static FrameBuffer* GetDefaultFramebuffer();
	};
}