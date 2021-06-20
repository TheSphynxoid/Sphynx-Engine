#pragma once
//#include "Core/Graphics/Window.h"
#include "Mesh.h"
#include "Vector.h"
#include "Material.h"
#include "Shader.h"

namespace Sphynx::Core {
	class IWindow;
}
namespace Sphynx::Core::Graphics {


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
		void Submit(Mesh* mesh, Material* mat, Vec3 Position, Vec4 Rotation) {
			OnSubmit(RenderObject(mesh, mat, Position, Rotation));
		}
		struct RenderObject {
			Mesh* mesh;
			Material* mat;
			Vec3 Position;
			Vec4 Rotation;
			RenderObject(Mesh* _mesh, Material* _mat, Vec3 Pos, Vec4 Rot) : mesh(_mesh), mat(_mat), Position(Pos), Rotation(Rot) {};
		};
	private:
		virtual void OnSubmit(RenderObject rend) = 0;
	};
}