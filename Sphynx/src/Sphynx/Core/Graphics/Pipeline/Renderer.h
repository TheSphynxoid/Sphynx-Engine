#pragma once
//#include "Core/Graphics/Window.h"
#include "Core/Module.h"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#ifdef GCC || CLANG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
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
		RenderObject(Mesh* _mesh, Material* _mat) : mesh(_mesh), mat(_mat) {};
	}RenderObject;
	//Base Class Of Renderers.
	class IRenderer
	{
	public:
		// Inherited via Module
		virtual void Start(IWindow* app) = 0;
		virtual void Render() = 0;
		virtual void Clear() = 0;
		virtual void SetDepthTesting(bool value) = 0;
		//Send Data,Shaders,Indexes,textures (Make A Object that encapsulates them all).
		//They are stored until deleted
		inline void Submit(const RenderObject& RO) {
			OnSubmit(RO);
		}

	private:
		virtual void OnSubmit(RenderObject rend) = 0;

	};
}
#ifdef GCC
#pragma GCC diagnostic pop
#endif // GCC