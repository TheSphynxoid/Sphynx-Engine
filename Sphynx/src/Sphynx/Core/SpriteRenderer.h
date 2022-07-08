#pragma once
#include "Component.h"
#include "Graphics/Pipeline/Material.h"
#include "glm/glm.hpp"
#include "Graphics/Pipeline/Mesh.h"
#include "Camera.h"

namespace Sphynx::Core {
	class SpriteRenderer : public Component
	{
	private:
		Graphics::Mesh* SpritePlane;
		Graphics::Material* SpriteMaterial;
		Graphics::Uniform* ModelUniform;
		glm::mat4 ModelMat = glm::mat4(1.0f);
		glm::vec2 Size;
		//void OnSceneChange(Sphynx::Core::OnSceneChange& e);
	public:
		SpriteRenderer(Graphics::Material* mat, glm::vec2 size);
		SpriteRenderer(Graphics::Texture* tex, glm::vec2 size);
		virtual void OnComponentAttach(GameObject* parent) override;
		virtual void Update() override;
	};
}
