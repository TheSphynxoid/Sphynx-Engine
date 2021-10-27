#pragma once
#include "Component.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "Core/Graphics/Pipeline/Mesh.h"
#include "Core/Graphics/Pipeline/Material.h"
#include "Transform.h"

namespace Sphynx {
	class MeshRenderer : public Component
	{
	private:
		Sphynx::Core::Graphics::Mesh* mesh;
		Sphynx::Core::Graphics::Material* Mat;
		Sphynx::Core::Graphics::RenderObject RO = Sphynx::Core::Graphics::RenderObject(nullptr, nullptr);
		glm::mat4 halfEquation = glm::mat4(0.0f);
		glm::mat4 FullEquation = glm::mat4(0.0f);
		Sphynx::Core::Graphics::Uniform* MVPUni = nullptr;
		virtual void OnComponentAttach(GameObject* parent) override;
		virtual void OnComponentDetach() override;
	public:
		MeshRenderer(Core::Graphics::Mesh* _mesh, Core::Graphics::Material* mat);
		void SetMaterial(Core::Graphics::Material* mat);
		Core::Graphics::Material* GetMaterial() { return Mat; };
		void SetMesh(Sphynx::Core::Graphics::Mesh* _mesh);
		Core::Graphics::Mesh* GetMesh() { return mesh; };
		virtual void Update() override;
	};
}