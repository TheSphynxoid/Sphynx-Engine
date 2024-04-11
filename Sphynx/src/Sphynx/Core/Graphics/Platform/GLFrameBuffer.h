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
		GLFrameBuffer(unsigned int id, int width, int height, std::initializer_list<Texture*> attachments = {})noexcept;
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
		GLFrameBuffer(int width, int height, std::initializer_list<Texture*> attachments = {})noexcept;
		void Bind(FrameBufferBinding b = FrameBufferBinding::ReadWrite)const noexcept override;
		void Unbind()const noexcept override;
		void Resize(unsigned int width, unsigned int height) override;
		Texture* GetColorAttachment(size_t index)const noexcept override { return ColorAttachments[index]; }
		void Invalidate() override;
		bool IsDefaultFrameBuffer()const noexcept override { return !ID; }
		int GetWidth()const noexcept override { return Width; }
		int GetHeight()const noexcept override { return Height; }
		~GLFrameBuffer() override;
		void AddColorAttachment(Texture* tex) override;
		void SetDepthStencilAttachment(Texture* tex) override;
		bool HasDepthAttachment()const noexcept override {
			return DepthAttachment;
		};
		Texture* GetDepthStencilAttachment()const noexcept override {
			return DepthAttachment;
		};
		//Useless.
		bool HasStencilAttachment()const noexcept override {
			return DepthAttachment;
		};
		bool IsValid()const noexcept override {
			return ColorAttachmentsCount;
		}
		void SetClearColor(glm::vec4 col)noexcept override;
		void Clear(ClearBuffer b)noexcept override;
		void Clear()noexcept override;
		void* GetNativeID()const noexcept override { return (void*)ID; }
		static FrameBuffer* GetDefaultFrameBuffer()noexcept;
	};
}