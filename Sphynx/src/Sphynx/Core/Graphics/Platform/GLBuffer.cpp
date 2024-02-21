#include "pch.h"
#include "GLBuffer.h"
#include "glad/glad.h"

using namespace Sphynx::Core::Graphics;

static inline GLenum MapAccessToGLenum(const Sphynx::Core::Graphics::MapAccess& _access) {
	switch (_access)
	{
	case Sphynx::Core::Graphics::MapAccess::Read:
		return GL_READ_ONLY;
	case Sphynx::Core::Graphics::MapAccess::Write:
		return GL_WRITE_ONLY;
	case Sphynx::Core::Graphics::MapAccess::ReadWrite:
		return GL_READ_WRITE;
	}
}
static inline GLenum UsageHintToGL(const UsageHint hint, const AccessHint accesshint) {
	switch (hint)
	{
	case Sphynx::Core::Graphics::UsageHint::Stream:
		return GL_STREAM_DRAW + (unsigned short)accesshint;
	case Sphynx::Core::Graphics::UsageHint::Static:
		return GL_STATIC_DRAW + (unsigned short)accesshint;
	case Sphynx::Core::Graphics::UsageHint::Dynamic:
		return GL_DYNAMIC_DRAW + (unsigned short)accesshint;
	default:
		//Assume Repeated read and write.
		return GL_DYNAMIC_DRAW + (unsigned short)accesshint;
	}
}

Sphynx::Core::Graphics::GL::GLBuffer::GLBuffer(size_t size, void* data, const UsageHint hint, const AccessHint accesshint)
{
	glCreateBuffers(1, &id);
	Usagehint = UsageHintToGL(hint, accesshint);
	glNamedBufferData(id, size, data, Usagehint);
}

Sphynx::Core::Graphics::GL::GLBuffer::~GLBuffer()
{
	Release();
}

void Sphynx::Core::Graphics::GL::GLBuffer::Bind() const noexcept
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void Sphynx::Core::Graphics::GL::GLBuffer::Unbind() const noexcept
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void* Sphynx::Core::Graphics::GL::GLBuffer::Map(const MapAccess access)
{
    return glMapNamedBuffer(id, MapAccessToGLenum(access));
}

void Sphynx::Core::Graphics::GL::GLBuffer::Unmap() noexcept
{
	glUnmapNamedBuffer(id);
}

void Sphynx::Core::Graphics::GL::GLBuffer::Reallocate(size_t size, void* data)
{
	glNamedBufferData(id, size, data, Usagehint);
}

void Sphynx::Core::Graphics::GL::GLBuffer::Invalidate() noexcept
{
	glInvalidateBufferData(id);
}

void Sphynx::Core::Graphics::GL::GLBuffer::SetData(const void* data, size_t size, size_t  offset)
{
	if (size + offset > Size) {
		Core_Error("size+offset is larger than the buffer size, Consider calling Reallocate first to get new buffer store.");
		return;
	}
	glNamedBufferSubData(id, offset, size, data);
}

void Sphynx::Core::Graphics::GL::GLBuffer::Release()
{
	glDeleteBuffers(1, &id);
	id = 0;
}
