#pragma once
#include "Core/Graphics/Pipeline/Renderer.h"
#include <map>
#include <vector>
#include "Events/WindowEvents.h"

namespace Sphynx::Core::Graphics::GL {
	class GLShader;
	class GLMaterial;
	class GLRenderer : public IRenderer
	{
		typedef std::vector<RenderObject> RenderObjectList;
		std::map<unsigned int,std::vector<RenderObject>*> RenderQueue;
		void RendererResizeEvent(Events::OnWindowResize& e);
		//To Avoid Re-initalization
		inline static bool DefaultsSet = false;
		//To Be Able To Delete Them.

		inline static GLShader* DefaultVertexShader;
		inline static GLShader* DefaultFragmentShader;
		inline static GLMaterial* DefaultMaterial;
		inline static RenderObject DefaultRenderObject = RenderObject(nullptr, nullptr, { 0,0,0 }, { 0,0,0,0 });
	public:
		// Inherited via IRenderer
		virtual void Start(IWindow* app) override;
		virtual void Render() override;
		virtual void Clear() override;
		virtual void OnSubmit(RenderObject rend) override;
		void SetClearColor(Vec4 color);
	};
}