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
		CamViewport.Width = e.Width;
		CamViewport.Height = e.Height;
		if (IsOrtho)
			ProjectionMatrix = glm::ortho(0.0f, (float)CamViewport.Width, 0.0f, (float)CamViewport.Height, NearClip, FarClip);
		else
			ProjectionMatrix = glm::perspective(glm::radians(FOV), (float)CamViewport.Width / CamViewport.Height, NearClip, FarClip);

		Events::GlobalEventSystem::GetInstance()->QueueEvent<OnFrameResize>
			(OnFrameResize({ CamViewport.Width, CamViewport.Height }, { e.Width, e.Height }, this, RenderTarget));
	}
}

void Sphynx::Camera::OnComponentAttach(GameObject* Parent)
{
	Events::GlobalEventSystem::GetInstance()->Subscribe<Camera, Events::OnWindowResize>(this, &Camera::ResizeWindow);
}

void Sphynx::Camera::OnComponentDetach()
{
	Events::GlobalEventSystem::GetInstance()->UnSubscribe<Camera, Events::OnWindowResize>(this, &Camera::ResizeWindow);
}

Sphynx::Camera::Camera() : IsOrtho(false), FOV(60.0f), NearClip(0.1f), FarClip(100.0f), 
	AspectRatio((float)GetMainWindow()->GetWidth() / (float)GetMainWindow()->GetHeight()),RenderTarget(Core::Graphics::FrameBuffer::GetDefaultFrameBuffer())
{
	CamViewport.Width = GetMainWindow()->GetWidth();
	CamViewport.Height = GetMainWindow()->GetHeight();
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), AspectRatio, 0.1f, 100.0f);
}

Sphynx::Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip) : IsOrtho(false), FOV(fov), NearClip(nearClip), FarClip(farClip),
	AspectRatio(aspectRatio), RenderTarget(Core::Graphics::FrameBuffer::GetDefaultFrameBuffer())
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
	if (GetTransform()->Changed) {
		ViewMatrix = glm::lookAt(GetTransform()->GetPosition(), GetTransform()->GetPosition() + glm::vec3(0, 0, -1),
			glm::rotate(GetTransform()->GetRotation(), glm::vec3(0,1,0)));
		
	}
	RenderTarget->Bind();
	RenderTarget->Clear();
	GetMainWindow()->GetRenderer()->SetViewPort(CamViewport);
	GetMainWindow()->GetRenderer()->Render();
	//GetMainWindow()->GetRenderer()->SetViewPort({ 0, 0, GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight() });
	RenderTarget->Unbind();
}

void Sphynx::Camera::SetFrameBuffer(Sphynx::Core::Graphics::FrameBuffer* fb)
{
	RenderTarget = fb;
	Events::GlobalEventSystem::GetInstance()->QueueEvent<OnFrameChange>(OnFrameChange(this, RenderTarget));
}

void Sphynx::Camera::SetViewport(Sphynx::Core::Graphics::Viewport v)
{
	if (RenderTarget) {
		RenderTarget->Resize(v.Width, v.Height);
		Events::GlobalEventSystem::GetInstance()->QueueEvent<OnFrameResize>
			(OnFrameResize({ CamViewport.Width, CamViewport.Height }, { v.Width, v.Height }, this, RenderTarget));
	}
	CamViewport = v;
}
