#include "pch.h"
#include "GLFrameBuffer.h"
#include "glad/glad.h"
#include "GLTexture.h"

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Core::Graphics;
using namespace Sphynx::Core::Graphics::GL;

void Sphynx::Core::Graphics::GL::GLFrameBuffer::Release()
{
	glDeleteFramebuffers(1, &ID);
	ID = 0;
	for (auto& tex : Attachements) {
		delete tex;
	}
}

Sphynx::Core::Graphics::GL::GLFrameBuffer::GLFrameBuffer(int width, int height, std::initializer_list<Texture*> attachments) : Width(width), Height(height)
{
	glCreateFramebuffers(1, &ID);
	Bind();
	for (auto& tex : attachments) {
		if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
			glNamedFramebufferTexture(ID, GL_DEPTH_STENCIL_ATTACHMENT, ((GLTexture*)tex)->TextureID, 0);
		}
		else {
			glNamedFramebufferTexture(ID, GL_COLOR_ATTACHMENT0 + ColorAttachments, ((GLTexture*)tex)->TextureID, 0);
			if (GL_MAX_COLOR_ATTACHMENTS - GL_COLOR_ATTACHMENT0 == ColorAttachments) {
				Core_Error("Max Color Attachment Reached");
			}
			else {
				ColorAttachments++;
			}
		}
	}
	Attachements = std::vector<Texture*>(attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Core_Error("ERROR: Framebuffer is not complete!");
	Unbind();
}

void Sphynx::Core::Graphics::GL::GLFrameBuffer::Bind(Sphynx::Core::Graphics::FrameBufferBinding b)
{
	switch (b)
	{
	case Sphynx::Core::Graphics::FrameBufferBinding::Read:
		glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
		break;
	case Sphynx::Core::Graphics::FrameBufferBinding::Write:
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ID);
		break;
	case Sphynx::Core::Graphics::FrameBufferBinding::ReadWrite:
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		break;
	}
}

void Sphynx::Core::Graphics::GL::GLFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Sphynx::Core::Graphics::GL::GLFrameBuffer::Resize(unsigned int width, unsigned int height)
{
	Width = width;
	Height = height;
	Invalidate();
}

void Sphynx::Core::Graphics::GL::GLFrameBuffer::Invalidate()
{
	if (!ID)
		glDeleteFramebuffers(1, &ID);
	glCreateFramebuffers(1, &ID);
	Bind();
	ColorAttachments = 0;
	for (auto& tex : Attachements) {
		auto err = glGetError();
		while (err != GL_NO_ERROR) {
			err = glGetError();
		}
		auto _t = Texture::Create(TextureType::Texture2D, Width, Height, tex->GetFormat(), tex->GetDataFormat());
		err = glGetError();
		while (err != GL_NO_ERROR) {
			err = glGetError();
		}
		(*((GLTexture*)tex)) = (GLTexture&&)*(GLTexture*)_t;
		if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
			glNamedFramebufferTexture(ID, GL_DEPTH_STENCIL_ATTACHMENT, ((GLTexture*)tex)->TextureID, 0);
		}
		else {
			glNamedFramebufferTexture(ID, GL_COLOR_ATTACHMENT0 + ColorAttachments, ((GLTexture*)tex)->TextureID, 0);
			if (GL_MAX_COLOR_ATTACHMENTS - GL_COLOR_ATTACHMENT0 == ColorAttachments) {
				Core_Error("Max Color Attachment Reached");
			}
			else {
				ColorAttachments++;
			}
		}
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Core_Error("ERROR: Framebuffer is not complete!");
	Unbind();
}

Sphynx::Core::Graphics::GL::GLFrameBuffer::~GLFrameBuffer()
{
	Release();
}
