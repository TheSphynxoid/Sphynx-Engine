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
		Graphics::Uniform* ModelUniform = nullptr;
		glm::mat4 ModelMat = glm::mat4(1.0f);
		glm::vec2 CenterOffset = glm::vec2(0.0f);
		glm::vec2 Size = glm::vec2(0.0f, 0.0f);
		//void OnSceneChange(Sphynx::Core::OnSceneChange& e);
	public:
		CompImpl(SpriteRenderer);
		SpriteRenderer();
		SpriteRenderer(Graphics::Material* mat, glm::vec2 size);
		SpriteRenderer(Graphics::Texture* tex, glm::vec2 size);
		void SetCenter(glm::vec2 Center);
		Graphics::Material* GetMaterial() {
			return SpriteMaterial;
		}
		virtual void OnComponentAttach(GameObject* parent) override;
		virtual void Update() override;
	};
}
