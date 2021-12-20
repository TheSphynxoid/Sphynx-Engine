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
	RenderTarget->Resize(e.Width, e.Height);
	Width = e.Width;
	Height = e.Height;
	if (IsOrtho)
		ProjectionMatrix = glm::ortho(0.0f, Width, 0.0f, Height, NearClip, FarClip);
	else
		ProjectionMatrix = glm::perspective(glm::radians(FOV), Width / Height, NearClip, FarClip);
}

void Sphynx::Camera::OnComponentAttach(GameObject* Parent)
{
}

void Sphynx::Camera::OnComponentDetach()
{
}

Sphynx::Camera::Camera() : IsOrtho(false), FOV(60.0f), NearClip(0.1f), FarClip(100.0f) , Width((float)GetMainWindow()->GetWidth())
,Height((float)GetMainWindow()->GetHeight())
{
	Events::GlobalEventSystem::GetInstance()->Subscribe<Camera, Events::OnWindowResize>(this, &Camera::ResizeWindow);
	DepthTex = Texture::Create(TextureType::Texture2D, GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight(), TextureFormat::Depth24_Stencil8,
		TextureDataFormat::UInt_24_8);
	ColorTex = Texture::Create(TextureType::Texture2D, GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight(), TextureFormat::RGBA,
		TextureDataFormat::UByte);
	RenderTarget = FrameBuffer::Create(GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight(), { DepthTex,ColorTex });
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)GetMainWindow()->GetWidth() / (float)GetMainWindow()->GetHeight(), 0.1f, 100.0f);
}

Sphynx::Camera::Camera(float fov, float nearClip, float farClip) : IsOrtho(false), FOV(fov), NearClip(nearClip), FarClip(farClip)
{
	DepthTex = Texture::Create(TextureType::Texture2D, GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight(), TextureFormat::Depth24_Stencil8,
		TextureDataFormat::UInt_24_8);
	ColorTex = Texture::Create(TextureType::Texture2D, GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight(), TextureFormat::RGBA,
		TextureDataFormat::UByte);
	RenderTarget = FrameBuffer::Create(GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight(), { DepthTex,ColorTex });
	ProjectionMatrix = glm::perspective(glm::radians(FOV), (float)GetMainWindow()->GetWidth() / (float)GetMainWindow()->GetHeight(), NearClip, FarClip);
}

Sphynx::Camera::Camera(float nearClip, float farClip, float width, float height) : IsOrtho(true), FOV(0), NearClip(nearClip), FarClip(farClip)
, Width(width), Height(height){
	DepthTex = Texture::Create(TextureType::Texture2D, GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight(), TextureFormat::Depth24_Stencil8,
		TextureDataFormat::UInt_24_8);
	ColorTex = Texture::Create(TextureType::Texture2D, GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight(), TextureFormat::RGBA,
		TextureDataFormat::UByte);
	RenderTarget = FrameBuffer::Create(GetMainWindow()->GetWidth(), GetMainWindow()->GetHeight(), { DepthTex,ColorTex });
	ProjectionMatrix = glm::ortho(0.0f, Width, 0.0f, Height, NearClip, FarClip);
	//ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
}

void Sphynx::Camera::Update()
{
	//ViewMatrix = glm::lookAt()
}