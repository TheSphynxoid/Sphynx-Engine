#pragma once
#include "Core/Graphics/Pipeline/Renderer.h"
#include <map>
#include <vector>
namespace Sphynx::Core::Graphics::GL {
	class GLRenderer : public IRenderer
	{
		typedef std::vector<RenderObject> RenderObjectList;
		std::map<unsigned int,std::vector<RenderObject>*> RenderQueue;
		// Inherited via IRenderer
		virtual void Start(IWindow* app) override;
		virtual void Render() override;
		virtual void Clear() override;
		virtual void OnSubmit(RenderObject rend) override;
	public:
		void SetClearColor(Vec4 color);
	};
}