#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "GameObject.h"
#include "Application.h"
#include "Core/Graphics/Window.h"
#include "SceneManager.h"

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Core::Graphics;

Sphynx::Camera* Cam;
IRenderer* Rend;

void Sphynx::MeshRenderer::Start()
{
	Cam = SceneManager::GetScene().GetPrimaryCamera();
	halfEquation = Cam->GetProjectionMatrix() * Cam->GetViewMatrix();
	FullEquation = halfEquation * GetTransform()->GetModelMatrix();
	//TODO: Remove Stupid Inclusion of Window.h
	Rend = GetMainWindow()->GetRenderer();
	MVPUni = Uniform::Create(&FullEquation[0], ShaderDataType::Mat4x4);
	Mat->SetUniform(MVPUni, "MVP");
	RO.mat = Mat;
	RO.mesh = mesh;
}

void Sphynx::MeshRenderer::OnComponentAttach(GameObject* Parent)
{
}

void Sphynx::MeshRenderer::OnComponentDetach()
{
}

Sphynx::MeshRenderer::MeshRenderer(Sphynx::Core::Graphics::Mesh* _mesh, Sphynx::Core::Graphics::Material* mat) : mesh(_mesh), Mat(mat)
{
}

void Sphynx::MeshRenderer::SetMaterial(Material* mat)
{
	Mat = mat;
}

void Sphynx::MeshRenderer::SetMesh(Sphynx::Core::Graphics::Mesh* _mesh)
{
	mesh = _mesh;
}

void Sphynx::MeshRenderer::Update()
{
	halfEquation = Cam->GetProjectionMatrix() * Cam->GetViewMatrix();
	FullEquation = halfEquation * GetTransform()->GetModelMatrix();
	MVPUni->SetData(&FullEquation[0]);
	Rend->Submit(RO);
}
