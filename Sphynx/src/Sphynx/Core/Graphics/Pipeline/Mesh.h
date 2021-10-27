#pragma once
#include <vector>
#include "Shader.h"

namespace Sphynx::Core::Graphics {
	enum class MeshType {
		//Changing Buffer
		Dynamic, 
		//Constant Buffer
		Static, 
		//GL Specific (Meaning : contents will be modified once and used at most a few times.)
		Stream
	};

	static size_t GetShaderDataTypeSize(ShaderDataType Type) {
		switch (Type)
		{
		case ShaderDataType::None:		 return 0;
		case ShaderDataType::Float:		 return sizeof(float);
		case ShaderDataType::Float2:	 return sizeof(float) * 2;
		case ShaderDataType::Float3:	 return sizeof(float) * 3;
		case ShaderDataType::Float4:	 return sizeof(float) * 4;
		case ShaderDataType::Double:	 return sizeof(double);
		case ShaderDataType::Double2:	 return sizeof(double) * 2;
		case ShaderDataType::Double3:	 return sizeof(double) * 3;
		case ShaderDataType::Double4:	 return sizeof(double) * 4;
		case ShaderDataType::Int:		 return sizeof(int);
		case ShaderDataType::Int2:		 return sizeof(int) * 2;
		case ShaderDataType::Int3:		 return sizeof(int) * 3;
		case ShaderDataType::Int4:		 return sizeof(int) * 4;
		case ShaderDataType::UInt:		 return sizeof(unsigned int);
		case ShaderDataType::UInt2:		 return sizeof(unsigned int) * 2;
		case ShaderDataType::UInt3:		 return sizeof(unsigned int) * 3;
		case ShaderDataType::UInt4:		 return sizeof(unsigned int) * 4;
		case ShaderDataType::Bool:		 return sizeof(bool);
		case ShaderDataType::Mat2x2:	 return sizeof(float) * 2 * 2;
		case ShaderDataType::Mat2x3:	 return sizeof(float) * 2 * 3;
		case ShaderDataType::Mat2x4:	 return sizeof(float) * 2 * 4;
		case ShaderDataType::Mat3x2:	 return sizeof(float) * 3 * 2;
		case ShaderDataType::Mat3x3:	 return sizeof(float) * 3 * 3;
		case ShaderDataType::Mat3x4:	 return sizeof(float) * 3 * 4;
		case ShaderDataType::Mat4x2:	 return sizeof(float) * 4 * 2;
		case ShaderDataType::Mat4x3:	 return sizeof(float) * 4 * 3;
		case ShaderDataType::Mat4x4:	 return sizeof(float) * 4 * 4;
		}
		return 0;
	}
	class BufferElement final{
	private:
		friend class BufferLayout;
		ShaderDataType Type = ShaderDataType::None;
		size_t Size = 0;
		size_t Offset = 0;
		bool Normalized = false;
	public:
		BufferElement()noexcept {};
		BufferElement(ShaderDataType type, bool normalized) : Type(type), Size(GetShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}
		size_t GetOffset()const noexcept { return Offset; };
		size_t GetSize()const noexcept { return Size; };
		bool IsNormalized()const noexcept { return Normalized; };
		ShaderDataType GetDataType()const noexcept { return Type; };
		unsigned int GetComponentCount()const noexcept {
			switch (Type)
			{
			case ShaderDataType::None:		return 0;
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Double:	return 1;
			case ShaderDataType::Double2:	return 2;
			case ShaderDataType::Double3:	return 3;
			case ShaderDataType::Double4:	return 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::UInt:		return 1;
			case ShaderDataType::UInt2:		return 2;
			case ShaderDataType::UInt3:		return 3;
			case ShaderDataType::UInt4:		return 4;
			case ShaderDataType::Bool:		return 1;
			case ShaderDataType::Mat2x2:	return 2;
			case ShaderDataType::Mat2x3:	return 3;
			case ShaderDataType::Mat2x4:	return 4;
			case ShaderDataType::Mat3x2:	return 2;
			case ShaderDataType::Mat3x3:	return 3;
			case ShaderDataType::Mat3x4:	return 4;
			case ShaderDataType::Mat4x2:	return 2;
			case ShaderDataType::Mat4x3:	return 3;
			case ShaderDataType::Mat4x4:	return 4;
			default:						return 0;
			}
		}
	};
	class BufferLayout final{
	private:
		std::vector<BufferElement> Elements = std::vector<BufferElement>();
		unsigned int Stride = 0;
	public:
		BufferLayout() {};
		BufferLayout(std::initializer_list<BufferElement>&& elements) : Elements(elements) {
			size_t offset = 0;
			for (auto& element : Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				Stride += element.Size;
			}
		};
		unsigned int GetStride() { return Stride; };
		std::vector<BufferElement>& GetElements() { return Elements; };
		size_t GetElementCount()const noexcept { return Elements.size(); };
		std::vector<BufferElement>::iterator begin()noexcept { return Elements.begin(); };
		std::vector<BufferElement>::iterator end()noexcept { return Elements.end(); };
		std::vector<BufferElement>::const_iterator begin() const noexcept { return Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const noexcept { return Elements.end(); }
		BufferElement& operator[](size_t pos)noexcept { return Elements[pos]; };
	};
	struct VertexBuffer {
		virtual ~VertexBuffer() = default;
		virtual void Bind()const = 0;
		virtual void Unbind()const = 0;
		virtual void SetData(const void* data, size_t offset, size_t size) = 0;
		virtual void SetDataLayout(BufferLayout layout) = 0;
		virtual BufferLayout GetLayout()const = 0;
		virtual size_t GetVertexBufferSize()const noexcept = 0;
		static VertexBuffer* Create(float* vertices, size_t size);
		static VertexBuffer* CreateEmpty(size_t size);
	};
	struct IndexBuffer {
		virtual ~IndexBuffer() = default;
		virtual void Bind()const = 0;
		virtual void Unbind()const = 0;
		virtual void SetData(const unsigned int* data, uint64_t count) = 0;
		virtual int GetCount()const noexcept = 0;
		static IndexBuffer* Create(unsigned int* indices, size_t size);
	};
	class Mesh
	{
	public:
		virtual ~Mesh() = default;
		static Mesh* CreateEmpty();
		static Mesh* Create(float* vertexes, size_t vertsize, unsigned int* indexes, size_t indexsize, MeshType meshtype);
		static Mesh* Create(VertexBuffer* VBuffer, IndexBuffer* IBuffer);
		static Mesh* Create(std::vector<VertexBuffer*> VBuffers, IndexBuffer* IBuffer);
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		virtual void AddVertexBuffer(VertexBuffer* VBuffer) = 0;
		virtual void AddVertexBuffers(std::vector<VertexBuffer*> _VBuffers) = 0;
		virtual void SetIndexBuffer(IndexBuffer* ibuf) = 0;
		virtual IndexBuffer* GetIndexBuffer()const = 0;
		virtual size_t GetIndexBufferSize()const noexcept = 0;
		virtual std::vector<VertexBuffer*> GetVertexBuffer()const noexcept = 0;
	};
}