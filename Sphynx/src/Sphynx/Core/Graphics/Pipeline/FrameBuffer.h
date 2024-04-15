#pragma once
#include "glm/vec4.hpp"

namespace Sphynx::Core::Graphics {
	class Texture;
	//Binding Point for the Framebuffer.
	enum class FrameBufferBinding : unsigned char {
		Read, Write, ReadWrite
	};
	enum class ClearBuffer : unsigned char {
		Color, Depth, Stencil
	};
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = 0;
		//Binds the Framebuffer for use.
		virtual void Bind(FrameBufferBinding b = FrameBufferBinding::ReadWrite)const noexcept = 0;
		//Unbinds the framebuffer (the default framebuffer will be used for reading or writing).
		virtual void Unbind()const noexcept = 0;
		//Gets the Specified color attachment.
		virtual Texture* GetColorAttachment(size_t index)const noexcept = 0;
		//Attaches a new Color Texture.
		virtual void AddColorAttachment(Texture* tex) = 0;
		//Sets the Depth/Stencil attachment.
		virtual void SetDepthStencilAttachment(Texture* tex) = 0;
		//return Whether the framebuffer has a depth texture.
		virtual bool HasDepthAttachment()const noexcept = 0;
		//Before using the Texture as a stencil Texture 
		//Check Whether HasStencilAttachment returns true or not (or Check the TextureType).
		virtual Texture* GetDepthStencilAttachment()const noexcept = 0;
		//return Whether the framebuffer has a stencil texture.
		virtual bool HasStencilAttachment()const noexcept = 0;
		//Causes the buffer to be Re-made.
		virtual void Invalidate() = 0;
		//Returns true if the framebuffer is the default one.
		virtual bool IsDefaultFrameBuffer()const noexcept = 0;
		//Returns a platform-specific ID
		virtual void* GetNativeID()const noexcept = 0;
		//Returns true if Framebuffer is valid and ready for use.
		virtual bool IsValid()const noexcept = 0;
		//Set Framebuffer Clear Color
		virtual void SetClearColor(glm::vec4 col)noexcept = 0;
		//Clears the Specified buffer
		virtual void Clear(ClearBuffer b)noexcept = 0;
		//Clears all buffers
		virtual void Clear()noexcept = 0;
		//Clears the Framebuffer binding
		static void BindDefault();
		//Creates a Framebuffer.
		static FrameBuffer* Create(std::initializer_list<Texture*> tex = {});
		//Returns the default framebuffer
		static FrameBuffer* GetDefaultFrameBuffer();
	};
}