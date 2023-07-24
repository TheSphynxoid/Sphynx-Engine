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
	for (auto& tex : ColorAttachments) {
		delete tex;
	}
}

Sphynx::Core::Graphics::GL::GLFrameBuffer::GLFrameBuffer(unsigned int id, int width, int height, std::initializer_list<Texture*> attachments)
{
	ID = id;
	Width = width;
	Height = Height;
	Bind();
	for (auto& tex : attachments) {
		if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
			if (!DepthAttachment) {
				DepthAttachment = tex;
			}
			else {
				Core_Error("Cannot Have Two Depth Attachments");
			}
		}
		else {
			if (GL_MAX_COLOR_ATTACHMENTS - GL_COLOR_ATTACHMENT0 == ColorAttachmentsCount) {
				Core_Error("Max Color Attachment Reached");
			}
			else {
				ColorAttachmentsCount++;
			}
			ColorAttachments.push_back(tex);
		}
	}
	Unbind();
}

Sphynx::Core::Graphics::GL::GLFrameBuffer::GLFrameBuffer(int width, int height, std::initializer_list<Texture*> attachments) : Width(width), Height(height)
{
	glCreateFramebuffers(1, &ID);
	Bind();
	for (auto& tex : attachments) {
		if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
			glNamedFramebufferTexture(ID, GL_DEPTH_STENCIL_ATTACHMENT, ((GLTexture*)tex)->TextureID, 0);
			if (!DepthAttachment) {
				DepthAttachment = tex;
			}
			else {
				Core_Error("Cannot Have Two Depth Attachments");
			}
		}
		else {
			glNamedFramebufferTexture(ID, GL_COLOR_ATTACHMENT0 + ColorAttachmentsCount, ((GLTexture*)tex)->TextureID, 0);
			if (GL_MAX_COLOR_ATTACHMENTS - GL_COLOR_ATTACHMENT0 == ColorAttachmentsCount) {
				Core_Error("Max Color Attachment Reached");
			}
			else {
				ColorAttachmentsCount++;
			}
			ColorAttachments.push_back(tex);
		}
	}
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
	ColorAttachmentsCount = 0;
	for (auto tex : ColorAttachments) {
		auto _t = Texture::Create(TextureType::Texture2D, Width, Height, 0, tex->GetFormat(), tex->GetDataFormat());
		//Weird ass Casts. First We cast into a GLTexture Pointer then we dereferance it then we move it to
		//tex after casting tex To GLTexture* from Texture* then we dereferance. It's looks distrubing.
		(*((GLTexture*)tex)) = (GLTexture&&)*(GLTexture*)_t;
		if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
			glNamedFramebufferTexture(ID, GL_DEPTH_STENCIL_ATTACHMENT, ((GLTexture*)tex)->TextureID, 0);
		}
		else {
			glNamedFramebufferTexture(ID, GL_COLOR_ATTACHMENT0 + ColorAttachmentsCount, ((GLTexture*)tex)->TextureID, 0);
			if (GL_MAX_COLOR_ATTACHMENTS - GL_COLOR_ATTACHMENT0 == ColorAttachmentsCount) {
				Core_Error("Max Color Attachment Reached");
			}
			else {
				ColorAttachmentsCount++;
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

void Sphynx::Core::Graphics::GL::GLFrameBuffer::AddColorAttachment(Texture* tex)
{
	if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
		Core_Error("Depth24_Stencil8 is not a valid color format");
		return;
	}
	glNamedFramebufferTexture(ID, GL_COLOR_ATTACHMENT0 + ColorAttachmentsCount, ((GLTexture*)tex)->TextureID, 0);
	ColorAttachmentsCount++;
	ColorAttachments.push_back(tex);
}

void Sphynx::Core::Graphics::GL::GLFrameBuffer::SetDepthStencilAttachment(Texture* tex)
{
	if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
		DepthAttachment = tex;
		glNamedFramebufferTexture(ID, GL_DEPTH_ATTACHMENT, ((GLTexture*)tex)->TextureID, 0);
	}
}

void Sphynx::Core::Graphics::GL::GLFrameBuffer::SetClearColor(glm::vec4 col)
{
	Bind();
	glClearColor(col.r, col.g, col.b, col.a);
	Unbind();
}

void Sphynx::Core::Graphics::GL::GLFrameBuffer::Clear(ClearBuffer b)
{
	switch (b)
	{
	case Sphynx::Core::Graphics::ClearBuffer::Color:
		glClear(GL_COLOR_BUFFER_BIT);
		break;
	case Sphynx::Core::Graphics::ClearBuffer::Depth:
		glClear(GL_DEPTH_BUFFER_BIT);
		break;
	case Sphynx::Core::Graphics::ClearBuffer::Stencil:
		glClear(GL_STENCIL_BUFFER_BIT);
		break;
	default:
		break;
	}
}

void Sphynx::Core::Graphics::GL::GLFrameBuffer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

FrameBuffer* Sphynx::Core::Graphics::GL::GLFrameBuffer::GetDefaultFramebuffer()
{
	static GLFrameBuffer def = GLFrameBuffer(0, 0, 0);
	static bool LazyInit = false;
	if(!LazyInit){
		def.ID = 0;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GLint dims[4] = { 0 };
		glGetIntegerv(GL_VIEWPORT, dims);
		def.Width = dims[2];
		def.Height = dims[3];

		LazyInit = true;
	}
	return &def;
}
