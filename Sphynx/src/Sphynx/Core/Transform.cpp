#include "pch.h"
#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/matrix.hpp"
#include "GameObject.h"

Sphynx::Transform::Transform()
{
	ModelMatrix = glm::mat4(1.0f);
}

Sphynx::Transform::Transform(glm::vec3 pos, glm::vec3 scale, glm::quat rot)
{
	ModelMatrix = glm::mat4(1.0f);
}

void Sphynx::Transform::Translate(glm::vec3 towards)
{
	ModelMatrix = glm::translate(ModelMatrix, towards);
	Position += towards;
}

void Sphynx::Transform::Rotate(float Angle, glm::vec3 axis)
{
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Angle), axis);
}

void Sphynx::Transform::Scale(glm::vec3 scale)
{
	ModelMatrix = glm::scale(ModelMatrix, scale);
}

void Sphynx::Transform::SetPosition(glm::vec3 to)
{
	ModelMatrix = glm::translate(glm::mat4(1.0f), to);
	Position = to;
}

void Sphynx::Transform::SetRotation(glm::vec3 rot)
{
}

glm::vec3& Sphynx::Transform::GetPosition()
{
	return Position;
}

Sphynx::Transform::~Transform()
{
}

void Sphynx::Transform::OnComponentAttach(GameObject* Parent)
{
}

void Sphynx::Transform::OnComponentDetach()
{
}

void Sphynx::Transform::Update()
{
	//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(10.0f), glm::vec3(0, 0, 1));
}
