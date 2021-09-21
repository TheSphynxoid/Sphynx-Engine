#pragma once
//#include "Core/Graphics/Window.h"
#include "Core/Module.h"
#include "Vector.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
namespace Sphynx::Core::Graphics {
	class Mesh;
	class Material;
	class Shader;
	//Information For the Rendering Engine
	typedef struct RenderObject {
	private:
		char padding = 0;//Padding
	public:
		Mesh* mesh;
		Material* mat;
		Vec3 Position;
		Vec4 Rotation;
		RenderObject(Mesh* _mesh, Material* _mat, Vec3 Pos, Vec4 Rot) : mesh(_mesh), mat(_mat), Position(Pos), Rotation(Rot) {};
	}RenderObject;
	//Base Class Of Renderers.
	class IRenderer
	{
	public:
		// Inherited via Module
		virtual void Start(IWindow* app) = 0;
		virtual void Render() = 0;
		virtual void Clear() = 0;
		//Send Data,Shaders,Indexes,Textures (Make A Object that encapsulates them all).
		//They are stored until deleted
		inline void Submit(const RenderObject& RO) {
			OnSubmit(RO);
		}

	private:
		virtual void OnSubmit(RenderObject rend) = 0;

	};
}
#pragma GCC diagnostic pop