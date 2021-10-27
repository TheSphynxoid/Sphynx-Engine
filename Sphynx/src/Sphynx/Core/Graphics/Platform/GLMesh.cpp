#include "pch.h"
#include "GLMesh.h"
#include "glad/glad.h"
// #include "GLFW/glfw3.h"

void SetVertexAttribs(Sphynx::Core::Graphics::VertexBuffer* buffs, unsigned int& Index) {
	buffs->Bind();
	//TODO: Check.
	for (auto& e : buffs->GetLayout()) {
		switch (e.GetDataType())
		{
		case Sphynx::Core::Graphics::ShaderDataType::None:
			Core_Error("Woah there, No Type ? We ain't Python here ! Attribute/Element Ignored.");
			//IDK, Error Handling ?
			Index--;
			break;
		case Sphynx::Core::Graphics::ShaderDataType::Float: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Float2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Float3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Float4:
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(Index, e.GetComponentCount(), GL_FLOAT, e.IsNormalized(),
				buffs->GetLayout().GetStride(), (void*)e.GetOffset());
			//Incrementing The index For the attribute
			Index++;
			break;
		case Sphynx::Core::Graphics::ShaderDataType::Double: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Double2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Double3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Double4:
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(Index, e.GetComponentCount(), GL_DOUBLE, e.IsNormalized(),
				buffs->GetLayout().GetStride(), (void*)e.GetOffset());
			//Incrementing The index For the attribute
			Index++;
			break;
		case Sphynx::Core::Graphics::ShaderDataType::Int: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Int2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Int3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Int4:
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(Index, e.GetComponentCount(), GL_INT, e.IsNormalized(),
				e.GetComponentCount() * sizeof(int), (void*)e.GetOffset());
			//Incrementing The index For the attribute
			Index++;
			break;
		case Sphynx::Core::Graphics::ShaderDataType::UInt: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::UInt2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::UInt3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::UInt4:
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(Index, e.GetComponentCount(), GL_UNSIGNED_INT, e.IsNormalized(),
				buffs->GetLayout().GetStride(), (void*)e.GetOffset());
			break;
		case Sphynx::Core::Graphics::ShaderDataType::Bool:
			glEnableVertexAttribArray(Index);
			glVertexAttribPointer(Index, e.GetComponentCount(), GL_FLOAT, e.IsNormalized(),
				buffs->GetLayout().GetStride(), (const void*)e.GetOffset());
			//Incrementing The index For the attribute
			Index++;
			break;
		case Sphynx::Core::Graphics::ShaderDataType::Mat2x2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat2x3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat2x4: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat3x2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat3x3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat3x4: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat4x2: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat4x3: [[fallthrough]];
		case Sphynx::Core::Graphics::ShaderDataType::Mat4x4:
			for (int i = 0; i < e.GetComponentCount(); i++) {
				glEnableVertexAttribArray(Index);
				glVertexAttribPointer(Index, e.GetComponentCount(), GL_FLOAT, e.IsNormalized(),
					buffs->GetLayout().GetStride(), (const void*)e.GetOffset());
				//modify the rate at which generic vertex attributes advance during instanced rendering.
				glVertexAttribDivisor(Index, 1);
				//Incrementing The index For the attribute
				Index++;
			}
			break;
		default:
			Core_Error("Unknown Shader Data Type : {0},{1},{2}", __FILE__, __FUNCTION__, __LINE__);
			break;
		}
	}
	buffs->Unbind();
}

GLenum MeshTypeToGLenum(Sphynx::Core::Graphics::MeshType meshtype) {
	switch (meshtype)
	{
	case Sphynx::Core::Graphics::MeshType::Dynamic:
		return GL_DYNAMIC_DRAW;
	case Sphynx::Core::Graphics::MeshType::Static:
		return GL_STATIC_DRAW;
	case Sphynx::Core::Graphics::MeshType::Stream:
		return GL_STREAM_DRAW;
	default:
		return GL_NONE;
	}
}

Sphynx::Core::Graphics::GL::GLMesh::GLMesh(GLMesh&& Mesh)noexcept
{
	std::swap(this->VAO, Mesh.VAO);
	std::swap(this->VAttribIndex, Mesh.VAttribIndex);
	std::swap(this->VBuffers, Mesh.VBuffers);
	std::swap(this->IBuffer, Mesh.IBuffer);
	std::swap(this->hasIndexArray, Mesh.hasIndexArray);
}

Sphynx::Core::Graphics::GL::GLMesh& Sphynx::Core::Graphics::GL::GLMesh::operator=(GLMesh&& Mesh)noexcept
{
	if (this != &Mesh) {
		Release();
		std::swap(this->VAO, Mesh.VAO);
		std::swap(this->VAttribIndex, Mesh.VAttribIndex);
		std::swap(this->VBuffers, Mesh.VBuffers);
		std::swap(this->IBuffer, Mesh.IBuffer);
		std::swap(this->hasIndexArray, Mesh.hasIndexArray);
	}
	return *this;
}

Sphynx::Core::Graphics::GL::GLMesh::~GLMesh()
{
	Release();
}

Sphynx::Core::Graphics::GL::GLMesh::GLMesh()
{
	glCreateVertexArrays(1, &VAO);
}

Sphynx::Core::Graphics::GL::GLMesh::GLMesh(float* vertexes, size_t vertsize, unsigned int* indexes, size_t indexsize, MeshType meshtype)
{
	//Create VAO
	glCreateVertexArrays(1, &VAO);
	//Create Vertex Buffer
	auto temp = new GLVertexBuffer(vertexes, vertsize);
	temp->SetDataLayout(BufferLayout({ BufferElement(ShaderDataType::Float3, false) }));
	VBuffers.push_back(temp);
	temp = nullptr;
	//Binding the Vertex Buffers in the VAO
	glBindVertexArray(VAO);
	auto vb = ((GLVertexBuffer*)VBuffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vb->BufferID);
	glBufferData(GL_ARRAY_BUFFER, vertsize, vertexes, MeshTypeToGLenum(meshtype));
	//Create and Bind Index Buffers
	if ((hasIndexArray = (indexes != nullptr))) {
		IBuffer = new GLIndexBuffer(indexes, indexsize / sizeof(*indexes));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBuffer->BufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexsize, indexes, MeshTypeToGLenum(meshtype));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
	VAttribIndex = 1;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Sphynx::Core::Graphics::GL::GLMesh::GLMesh(std::vector<VertexBuffer*> VertBuf, IndexBuffer* IndexBuf)
	: VBuffers(VertBuf), IBuffer(static_cast<GLIndexBuffer*>(IndexBuf))
{
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	if ((hasIndexArray = (IndexBuf != nullptr))) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLIndexBuffer*)IndexBuf)->BufferID);
	}
	//Attributes For Vertex Buffers
	for (auto* buffs : VertBuf)
	{
		//Attributes
		SetVertexAttribs(buffs, VAttribIndex);
	}
	//Unbinding The VAO for Clean-ness
	glBindVertexArray(0);
}

void Sphynx::Core::Graphics::GL::GLMesh::Bind()const
{
	glBindVertexArray(VAO);
}

void Sphynx::Core::Graphics::GL::GLMesh::UnBind()const
{
	glBindVertexArray(0);
}

void Sphynx::Core::Graphics::GL::GLMesh::AddVertexBuffer(VertexBuffer* VBuffer)
{
	Bind();
	VBuffers.push_back(VBuffer);
	SetVertexAttribs(VBuffer, VAttribIndex);
	UnBind();
}

void Sphynx::Core::Graphics::GL::GLMesh::AddVertexBuffers(std::vector<VertexBuffer*> _VBuffers)
{
	Bind();
	VBuffers.insert(VBuffers.end(), _VBuffers.begin(), _VBuffers.end());
	for (auto& vb : _VBuffers) {
		SetVertexAttribs(vb, VAttribIndex);
	}
	UnBind();
}

void Sphynx::Core::Graphics::GL::GLMesh::SetIndexBuffer(IndexBuffer* ibuf)
{
	hasIndexArray = true;
	IBuffer = static_cast<GLIndexBuffer*>(ibuf);
}

void Sphynx::Core::Graphics::GL::GLMesh::Release()
{
	if (this->hasIndexArray) {
		this->UnBind();
		IBuffer->Release();
	}
	glDeleteVertexArrays(1, &VAO);
	VAO = 0;
	for (auto v : VBuffers) { static_cast<GLVertexBuffer*>(v)->Release(); }
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::Release()
{
	glDeleteBuffers(1, &BufferID);
	BufferID = 0;
	Size = 0;

}

Sphynx::Core::Graphics::GL::GLVertexBuffer::GLVertexBuffer(size_t size) : Size(size)
{
	glCreateBuffers(1, &BufferID);
	glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Sphynx::Core::Graphics::GL::GLVertexBuffer::GLVertexBuffer(float* verts, size_t size) : Size(size)
{
	glCreateBuffers(1, &BufferID);
	glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	glBufferData(BufferID, size, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Sphynx::Core::Graphics::GL::GLVertexBuffer::GLVertexBuffer(GLVertexBuffer&& vbuf) noexcept
{
	std::swap(this->BufferID, vbuf.BufferID);
	std::swap(this->Layout, vbuf.Layout);
	std::swap(this->Size, vbuf.Size);
}

Sphynx::Core::Graphics::GL::GLVertexBuffer& Sphynx::Core::Graphics::GL::GLVertexBuffer::operator=(GLVertexBuffer&& vbuf) noexcept
{
	if (this != &vbuf) {
		Release();
		std::swap(this->BufferID, vbuf.BufferID);
		std::swap(this->Layout, vbuf.Layout);
		std::swap(this->Size, vbuf.Size);
	}
	return *this;
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::Bind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferID);
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::Unbind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sphynx::Core::Graphics::GL::GLVertexBuffer::SetData(const void* data, size_t offset, size_t size)noexcept
{
	if (size <= this->Size) {
		glBindBuffer(GL_ARRAY_BUFFER, BufferID);
		glBufferSubData(BufferID, offset, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, BufferID);
		glBufferData(BufferID, size, data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		this->Size = size;
	}
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::Release()
{
	glDeleteBuffers(1, &BufferID);
	BufferID = 0;
}

Sphynx::Core::Graphics::GL::GLIndexBuffer::GLIndexBuffer(uint32_t count)noexcept : Count(count)
{
	glCreateBuffers(1, &BufferID);
	// GL_ELEMENT_ARRAY_BUFFER need a bound VAO to be valid.
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
	glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	glBufferData(BufferID, count * sizeof(UINT64), 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Sphynx::Core::Graphics::GL::GLIndexBuffer::GLIndexBuffer(unsigned int* indices, size_t count)noexcept : Count(count)
{
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Sphynx::Core::Graphics::GL::GLIndexBuffer::GLIndexBuffer(unsigned int* indices, size_t count)noexcept : Count(count)
{
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Sphynx::Core::Graphics::GL::GLIndexBuffer::GLIndexBuffer(GLIndexBuffer&& ibuf) noexcept
{
	std::swap(this->BufferID, ibuf.BufferID);
}

Sphynx::Core::Graphics::GL::GLIndexBuffer& Sphynx::Core::Graphics::GL::GLIndexBuffer::operator=(GLIndexBuffer&& ibuf) noexcept
{
	if (this != &ibuf) {
		Release();
		std::swap(this->BufferID, ibuf.BufferID);
	}
	return *this;
}

Sphynx::Core::Graphics::GL::GLIndexBuffer::~GLIndexBuffer()
{
	Release();
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID);
}

void Sphynx::Core::Graphics::GL::GLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Sphynx::Core::Graphics::GL::GLIndexBuffer::SetData(const unsigned int* data, uint64_t count)
{
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
	Unbind();
}