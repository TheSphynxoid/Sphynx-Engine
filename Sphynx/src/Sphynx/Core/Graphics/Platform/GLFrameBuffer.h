#pragma once
#include "Core/Graphics/Pipeline/FrameBuffer.h"

namespace Sphynx::Core::Graphics {
	class Texture;
}
namespace Sphynx::Core::Graphics::GL {
	class GLTexture;
	class GLFrameBuffer final : public FrameBuffer
	{
	private:
		unsigned int ID = 0;
		int Width = 0, Height = 0;
		int ColorAttachmentsCount = 0;
		Texture* DepthAttachment = nullptr;
		std::vector<Texture*> ColorAttachments;
		void Release();
		//Used to Create the Default framebuffer
		GLFrameBuffer(unsigned int id, int width, int height, std::initializer_list<Texture*> attachments = {});
	public:
		GLFrameBuffer() = delete;
		GLFrameBuffer(const GLFrameBuffer&) = delete;
		GLFrameBuffer& operator=(const GLFrameBuffer&) = delete;
		GLFrameBuffer(GLFrameBuffer&& fb) noexcept
		{
			std::swap(ID, fb.ID);
			std::swap(Width, fb.Width);
			std::swap(Height, fb.Height);
			std::swap(ColorAttachmentsCount, fb.ColorAttachmentsCount);
			std::swap(DepthAttachment, fb.DepthAttachment);
			std::swap(ColorAttachments, fb.ColorAttachments);
		}
		GLFrameBuffer& operator=(GLFrameBuffer&& fb)noexcept
		{
			if(this != &fb)
			{
				Release();
				std::swap(ID, fb.ID);
				std::swap(Width, fb.Width);
				std::swap(Height, fb.Height);
				std::swap(ColorAttachmentsCount, fb.ColorAttachmentsCount);
				std::swap(DepthAttachment, fb.DepthAttachment);
				std::swap(ColorAttachments, fb.ColorAttachments);
			}
			return *this;
		}
		GLFrameBuffer(int width, int height, std::initializer_list<Texture*> attachments = {});
		void Bind(FrameBufferBinding b = FrameBufferBinding::ReadWrite) override;
		void Unbind() override;
		void Resize(unsigned int width, unsigned int height) override;
		Texture* GetColorAttachment(size_t index) override { return ColorAttachments[index]; }
		void Invalidate() override;
		bool IsDefaultFrameBuffer() override { return !ID; }
		int GetWidth() override { return Width; }
		int GetHeight() override { return Height; }
		~GLFrameBuffer() override;
		void AddColorAttachment(Texture* tex) override;
		void SetDepthStencilAttachment(Texture* tex) override;
		bool HasDepthAttachment() override {
			return DepthAttachment;
		};
		Texture* GetDepthStencilAttachment() override {
			return DepthAttachment;
		};
		//Useless.
		bool HasStencilAttachment() override {
			return DepthAttachment;
		};
		bool IsValid() override {
			return ColorAttachmentsCount;
		}
		void SetClearColor(glm::vec4 col) override;
		void Clear(ClearBuffer b) override;
		void Clear() override;
		void* GetNativeID() override { return (void*)ID; }
		static FrameBuffer* GetDefaultFrameBuffer();
	};
}