#pragma once

namespace Sphynx::Core::Graphics {
	enum class MeshType {
		//Changing Buffer
		Dynamic, 
		//Constant Buffer
		Static, 
		//GL Specific (Meaning : contents will be modified once and used at most a few times.)
		Stream
	};
	class Mesh
	{
	public:
		virtual void Create(float* vertexes, size_t vertsize, int* indexes, size_t indexsize, MeshType meshtype) = 0;
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual bool IsValid() = 0;
	};
}