#pragma once
#include "Events/Event.h"
#include "glm/vec4.hpp"

namespace Sphynx::Core::Graphics {
	class Texture;
	//Binding Point for the Framebuffer.
	enum class FrameBufferBinding {
		Read, Write, ReadWrite
	};
	enum class ClearBuffer {
		Color, Depth, Stencil
	};
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		//Binds the Framebuffer for use.
		virtual void Bind(FrameBufferBinding b = FrameBufferBinding::ReadWrite) = 0;
		//Unbinds the framebuffer (the default framebuffer will be used for reading or writing).
		virtual void Unbind() = 0;
		//Resize the Framebuffer and attachement. Calling this Invalidates the FrameBuffer.
		virtual void Resize(unsigned int width, unsigned int height) = 0;
		//Gets the Specified color attachement.
		virtual Texture* GetColorAttachment(size_t index) = 0;
		//Attaches a new Color Texture.
		virtual void AddColorAttachment(Texture* tex) = 0;
		//Sets the Depth/Stencil Attachement
		virtual void SetDepthStencilAttachment(Texture* tex) = 0;
		//return Whether the framebuffer has a depth texture.
		virtual bool HasDepthAttachment() = 0;
		//Before using the Texture as a stencil Texture 
		//Check Whether HasStencilAttachment returns true or not (or Check the TextureType).
		virtual Texture* GetDepthStencilAttachment() = 0;
		//return Whether the framebuffer has a stencil texture.
		virtual bool HasStencilAttachment() = 0;
		//Causes the buffer to be Re-made.
		virtual void Invalidate() = 0;
		//Returns true if the framebuffer is the default one.
		virtual bool IsDefaultFrameBuffer() = 0;
		//Returns the Width of the Texture Within the Framebuffer.
		virtual int GetWidth() = 0;
		//Returns the Height of the Texture Within the Framebuffer.
		virtual int GetHeight() = 0;
		//Returns a platform-specific ID
		virtual void* GetNativeID() = 0;
		//Returns true if Framebuffer is valid and ready for use.
		virtual bool IsValid() = 0;
		//Set Framebuffer Clear Color
		virtual void SetClearColor(glm::vec4 col) = 0;
		//Clears the Specified buffer
		virtual void Clear(ClearBuffer b) = 0;
		//Clears all buffers
		virtual void Clear() = 0;
		//Creates a Framebuffer.
		static void BindDefault();
		static FrameBuffer* Create(int width, int height, std::initializer_list<Texture*> tex = {});
		static FrameBuffer* GetDefaultFrameBuffer();
	};
}