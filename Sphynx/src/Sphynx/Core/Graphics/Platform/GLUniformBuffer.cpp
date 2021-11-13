#include "pch.h"
#include "GLUniformBuffer.h"
#include "glad/glad.h"

using namespace Sphynx::Core::Graphics::GL;

void Sphynx::Core::Graphics::GL::GLUniformBuffer::Release()
{
	glDeleteBuffers(1, &Buffer);
	Size = 0;
}

Sphynx::Core::Graphics::GL::GLUniformBuffer::GLUniformBuffer(size_t Size) : Size(Size)
{
	//Create The Buffer.
	glCreateBuffers(1, &Buffer);
	//Binding and Setting Data.
	glBindBuffer(GL_UNIFORM_BUFFER, Buffer);
	glBufferData(GL_UNIFORM_BUFFER, Size, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Sphynx::Core::Graphics::GL::GLUniformBuffer::GLUniformBuffer(void* Data, size_t Size) : Size(Size)
{
	auto err = glGetError();
	while (err != GL_NO_ERROR) {
		Core_Error("{0},{1},{2}", err, __FILE__, __LINE__);
		err = glGetError();
	}
	//Create The Buffer.
	glCreateBuffers(1, &Buffer);
	//Binding and Setting Data.
	glBindBuffer(GL_UNIFORM_BUFFER, Buffer);
	glBufferData(GL_UNIFORM_BUFFER, Size, Data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Sphynx::Core::Graphics::GL::GLUniformBuffer::GLUniformBuffer(GLUniformBuffer&& Ubuf) noexcept
{
	Release();
	std::swap(this->Buffer, Ubuf.Buffer);
	std::swap(this->Size, Ubuf.Size);
}

Sphynx::Core::Graphics::GL::GLUniformBuffer& Sphynx::Core::Graphics::GL::GLUniformBuffer::operator=(GLUniformBuffer&& Ubuf) noexcept
{
	if (this != &Ubuf) {
		Release();
		std::swap(this->Buffer, Ubuf.Buffer);
		std::swap(this->Size, Ubuf.Size);
	}
	return *this;
}

void Sphynx::Core::Graphics::GL::GLUniformBuffer::SetData(void* data, size_t Size)
{
	if (Size <= Size)
		glNamedBufferSubData(Buffer, 0, Size, data);
	else
		glNamedBufferData(Buffer, Size, data, GL_DYNAMIC_DRAW);
}

void Sphynx::Core::Graphics::GL::GLUniformBuffer::SetData(void* data, size_t offset, size_t Size)
{
	if (Size + offset <= Size)
		glNamedBufferSubData(Buffer, offset, Size, data);
}

void Sphynx::Core::Graphics::GL::GLUniformBuffer::Bind(unsigned int BindingPoint)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, BindingPoint, Buffer);
}

void Sphynx::Core::Graphics::GL::GLUniformBuffer::BindRange(unsigned int BindingPoint,size_t start, size_t Size)
{
	glBindBufferRange(GL_UNIFORM_BUFFER, BindingPoint, Buffer, start, Size);
}

GLenum GetGLAccess(Sphynx::Core::Graphics::BufferAccess ac) {
	switch (ac)
	{
	case Sphynx::Core::Graphics::BufferAccess::Read:		return GL_READ_ONLY;
	case Sphynx::Core::Graphics::BufferAccess::Write:		return GL_WRITE_ONLY;
	case Sphynx::Core::Graphics::BufferAccess::ReadWrite:	return GL_READ_WRITE;
	default:												return GL_READ_WRITE;
	}
}

void* Sphynx::Core::Graphics::GL::GLUniformBuffer::MapBuffer(BufferAccess access)
{
	MappedAddress = glMapNamedBuffer(Buffer, GetGLAccess(access));
	return MappedAddress;
}

void Sphynx::Core::Graphics::GL::GLUniformBuffer::UnMapBuffer()
{
	glUnmapNamedBuffer(Buffer);
	MappedAddress = nullptr;
}
