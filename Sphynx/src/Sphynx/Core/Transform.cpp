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
	Position += towards;
	CalcModelMatrix();
}

void Sphynx::Transform::Rotate(float Angle, glm::vec3 axis)
{
	Rotation = glm::rotate(Rotation, Angle, axis);
	CalcModelMatrix();
}

void Sphynx::Transform::Scale(glm::vec3 scale)
{
	Scaling += scale;
	CalcModelMatrix();
}

void Sphynx::Transform::SetPosition(glm::vec3 to)
{
	Position = to;
	CalcModelMatrix();
}

void Sphynx::Transform::SetRotation(glm::vec3 rot)
{
	Rotation = glm::qua(glm::radians(rot));
	CalcModelMatrix();
}

void Sphynx::Transform::SetScale(glm::vec3 scale)
{
	Scaling = scale;
	CalcModelMatrix();
}

void Sphynx::Transform::SetOrigin(glm::vec3 origin)
{
	Origin = origin;
	CalcModelMatrix();
}

glm::vec3 Sphynx::Transform::GetPosition()
{
	return Position;
}

glm::vec3 Sphynx::Transform::GetScale()
{
	return Scaling;
}

glm::quat Sphynx::Transform::GetRotation()
{
	return Rotation;
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

void Sphynx::Transform::CalcModelMatrix()
{
	//Translate to Position.
	ModelMatrix = glm::translate(glm::mat4(1.0f), Position);
	//Offset the origin for rotation.
	ModelMatrix = glm::translate(ModelMatrix, Origin);
	//Apply Rotation.
	ModelMatrix = ModelMatrix * (glm::mat4)Rotation;
	//Removing the origin offset. (returning the origin to (0,0,0)).
	ModelMatrix = glm::translate(ModelMatrix, -Origin);
	//Apply Scaling.
	ModelMatrix = glm::scale(ModelMatrix, Scaling);

}

void Sphynx::Transform::Update()
{
	if (Changed) {
		CalcModelMatrix();
	}
}
