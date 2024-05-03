#include "pch.h"
#include "GLFrameBuffer.h"
#include "glad/glad.h"
#include "GLTexture.h"

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Core::Graphics;
using namespace Sphynx::Core::Graphics::GL;

void GLFrameBuffer::Release()
{
	glDeleteFramebuffers(1, &ID);
	ID = 0;
	for (const auto& tex : ColorAttachments) {
		delete tex;
	}
}

GLFrameBuffer::GLFrameBuffer(unsigned int id, std::initializer_list<Texture*> attachments)noexcept
{
	ID = id;
	GLFrameBuffer::Bind();
	for (auto& tex : attachments) {
		if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
			if (!DepthAttachment) {
				DepthAttachment = tex;
			}
			else {
				Core_Error("Cannot Have Two Depth Attachments.");
			}
		}
		else {
			ColorAttachmentsCount++;
			ColorAttachments.push_back(tex);
		}
	}
	GLFrameBuffer::Unbind();
}

GLFrameBuffer::GLFrameBuffer(std::initializer_list<Texture*> attachments)noexcept
{
	static auto maxAttachment = []()->GLint {GLint val; glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &val); return val; }();
	glCreateFramebuffers(1, &ID);
	GLFrameBuffer::Bind();
	for (auto& tex : attachments) {
		if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
			glNamedFramebufferTexture(ID, GL_DEPTH_STENCIL_ATTACHMENT, ((GLTexture*)tex)->TextureID, 0);
			if (!DepthAttachment) {
				DepthAttachment = tex;
			}
			else {
				Core_Error("Cannot Have Two Depth Attachments.");
			}
		}
		else {
			glNamedFramebufferTexture(ID, GL_COLOR_ATTACHMENT0 + ColorAttachmentsCount, ((GLTexture*)tex)->TextureID, 0);
			if (maxAttachment - GL_COLOR_ATTACHMENT0 == ColorAttachmentsCount) {
				Core_Error("Max Color Attachment Reached.");
			}
			else {
				ColorAttachmentsCount++;
			}
			ColorAttachments.push_back(tex);
		}
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Core_Error("ERROR: Framebuffer is not complete!");
	GLFrameBuffer::Unbind();
}

void GLFrameBuffer::Bind(FrameBufferBinding b)const noexcept
{
	switch (b)
	{
	case FrameBufferBinding::Read:
		glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
		break;
	case FrameBufferBinding::Write:
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ID);
		break;
	case FrameBufferBinding::ReadWrite:
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		break;
	}
}

void GLFrameBuffer::Unbind()const noexcept
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::Invalidate()
{
	if (!ID)
		glDeleteFramebuffers(1, &ID);
	glCreateFramebuffers(1, &ID);
	Bind();
	ColorAttachmentsCount = 0;
	for (const auto tex : ColorAttachments) {
		if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
			glNamedFramebufferTexture(ID, GL_DEPTH_STENCIL_ATTACHMENT, ((GLTexture*)tex)->TextureID, 0);
		}
		else {
			glNamedFramebufferTexture(ID, GL_COLOR_ATTACHMENT0 + ColorAttachmentsCount, ((GLTexture*)tex)->TextureID, 0);
			ColorAttachmentsCount++;
		}
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Core_Error("ERROR: Framebuffer is not complete.");
	Unbind();
}

Texture* Sphynx::Core::Graphics::GL::GLFrameBuffer::ReplaceAttachment(Texture* tex, unsigned int index, int mipmapLevel) noexcept
{
	if (index <= ColorAttachmentsCount) {
		Texture* old = ColorAttachments[index];
		glNamedFramebufferTexture(ID, GL_COLOR_ATTACHMENT0 + index, (GLuint)tex->GetNativeID(), mipmapLevel);
		return old;
	}
	return nullptr;
}

GLFrameBuffer::~GLFrameBuffer()
{
	Release();
}

void GLFrameBuffer::AddColorAttachment(Texture* tex)
{
	if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
		Core_Error("Depth/Stencil Textures are not valid color format.");
		return;
	}
	glNamedFramebufferTexture(ID, GL_COLOR_ATTACHMENT0 + ColorAttachmentsCount, ((GLTexture*)tex)->TextureID, 0);
	ColorAttachmentsCount++;
	ColorAttachments.push_back(tex);
}

void GLFrameBuffer::SetDepthStencilAttachment(Texture* tex)
{
	if (tex->GetFormat() == TextureFormat::Depth24_Stencil8) {
		DepthAttachment = tex;
		glNamedFramebufferTexture(ID, GL_DEPTH_ATTACHMENT, ((GLTexture*)tex)->TextureID, 0);
	}
}

void GLFrameBuffer::SetClearColor(glm::vec4 col)noexcept
{
	Bind();
	glClearColor(col.r, col.g, col.b, col.a);
	Unbind();
}

void GLFrameBuffer::Clear(ClearBuffer b)noexcept
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
	}
}

void GLFrameBuffer::Clear()noexcept
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

FrameBuffer* GLFrameBuffer::GetDefaultFrameBuffer()noexcept
{
	static auto def = GLFrameBuffer(0);
	return &def;
}
