#include "pch.h"
#include "Camera.h"
#include "Application.h"
#include "Core/Graphics/Window.h"

Sphynx::GameObject* Sphynx::Camera::PrimaryCamera = new Sphynx::GameObject();

void Sphynx::Camera::OnComponentAttach(GameObject* Parent)
{
	//this->GetGameObject()->GetTransform()->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)));
}

void Sphynx::Camera::OnComponentDetach()
{
}

Sphynx::Camera::Camera() : IsOrtho(false)
{
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)GetMainWindow()->GetWidth() / (float)GetMainWindow()->GetHeight(), 0.1f, 100.0f);
}

Sphynx::Camera::Camera(float FOV, float NearClip, float FarClip) : IsOrtho(false)
{
	ProjectionMatrix = glm::perspective(glm::radians(FOV), (float)GetMainWindow()->GetWidth() / (float)GetMainWindow()->GetHeight(), NearClip, FarClip);
}

Sphynx::Camera::Camera(float NearClip, float FarClip, float Width, float Height) : IsOrtho(true)
{
	ProjectionMatrix = glm::ortho(0.0f, Width, 0.0f, Height, NearClip, FarClip);
	//ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
}

void Sphynx::Camera::Update()
{
	//ViewMatrix = glm::lookAt()
}

//Singleton.
Sphynx::GameObject* Sphynx::Camera::GetPrimaryCamera()
{
	return PrimaryCamera;
}
