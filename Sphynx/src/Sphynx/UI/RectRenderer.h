#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include "Core/Graphics/Pipeline/Renderer.h"

namespace Sphynx::Core::Graphics {
	class Texture;
	class Mesh;
	class Material;
}

namespace Sphynx::UI {
	class RectRenderer : public Component
	{
	private:
		Core::Graphics::Mesh* rectMesh;
		Core::Graphics::Material* rectMaterial;
		Core::Graphics::RenderObject RO = { 0,0 };
		virtual void OnComponentAttach(GameObject* parent) override;
		virtual void OnComponentDetach() override;
	public:
		virtual void Update() override;
		RectRenderer(glm::vec2 bounds, Core::Graphics::Texture* texture);
		Core::Graphics::Material* GetMaterial() {
			return rectMaterial;
		};
	};
}
