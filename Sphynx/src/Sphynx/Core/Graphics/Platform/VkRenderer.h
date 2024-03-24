#pragma once
#include "Core/Graphics/Pipeline/Renderer.h"

namespace Sphynx::Core::Graphics::Vulkan {
	class VkRenderer : public IRenderer
	{
		virtual void Start(IWindow* app) override;
		virtual void Render() override;
		virtual void Clear() override;
		virtual void SetDepthTesting(bool value) override;
		virtual void SetViewport(Viewport view) override;
		virtual const Viewport& GetViewport() override;
	private:
		virtual void OnSubmit(RenderObject rend) override;
	};
}
