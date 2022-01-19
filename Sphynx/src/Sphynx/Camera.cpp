#include "pch.h"
#include "Camera.h"
#include "Application.h"
#include "Core/Graphics/Window.h"
#include "Core/Graphics/Pipeline/FrameBuffer.h"
#include "Core/Graphics/Pipeline/Texture.h"
#include "Core/Graphics/Pipeline/Mesh.h"
#include "Core/MeshRenderer.h"
#include "Events/WindowEvents.h"

using namespace Sphynx::Core::Graphics;

void Sphynx::Camera::ResizeWindow(Events::OnWindowResize& e)
{
	if (RenderTarget == nullptr || RenderTarget->IsDefaultFrameBuffer()) {
		RenderTarget->Resize(e.Width, e.Height);
		CamViewport.Width = e.Width;
		CamViewport.Height = e.Height;
		if (IsOrtho)
			ProjectionMatrix = glm::ortho(0.0f, (float)CamViewport.Width, 0.0f, (float)CamViewport.Height, NearClip, FarClip);
		else
			ProjectionMatrix = glm::perspective(glm::radians(FOV), (float)CamViewport.Width / CamViewport.Height, NearClip, FarClip);
	}
}

void Sphynx::Camera::OnComponentAttach(GameObject* Parent)
{
}

void Sphynx::Camera::OnComponentDetach()
{
}

Sphynx::Camera::Camera() : IsOrtho(false), FOV(60.0f), NearClip(0.1f), FarClip(100.0f)
{
	CamViewport.Width = GetMainWindow()->GetWidth();
	CamViewport.Height = GetMainWindow()->GetHeight();
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)GetMainWindow()->GetWidth() / (float)GetMainWindow()->GetHeight(), 0.1f, 100.0f);
}

Sphynx::Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip) : IsOrtho(false), FOV(fov), NearClip(nearClip), FarClip(farClip)
{
	ProjectionMatrix = glm::perspective(glm::radians(FOV), aspectRatio, NearClip, FarClip);
}

//Sphynx::Camera::Camera(float nearClip, float farClip, float width, float height) : IsOrtho(true), FOV(0), NearClip(nearClip), FarClip(farClip)
//, Width(width), Height(height){
//	ProjectionMatrix = glm::ortho(0.0f, Width, 0.0f, Height, NearClip, FarClip);
//	//ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
//}

void Sphynx::Camera::Update()
{
	//ViewMatrix = glm::lookAt()
	if (RenderTarget) {
		RenderTarget->Bind();
		RenderTarget->Clear();
		GetMainWindow()->GetRenderer()->SetViewPort(CamViewport);
		GetMainWindow()->GetRenderer()->Render();
		GetMainWindow()->GetRenderer()->SetViewPort({ 0, 0, GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight() });
		RenderTarget->Unbind();
	}
}

void Sphynx::Camera::SetFrameBuffer(Sphynx::Core::Graphics::FrameBuffer* fb)
{
	RenderTarget = fb;
}

void Sphynx::Camera::SetViewport(Sphynx::Core::Graphics::Viewport v)
{
	CamViewport = v;
	if (RenderTarget) {
		RenderTarget->Resize(v.Width, v.Height);
	}
}
