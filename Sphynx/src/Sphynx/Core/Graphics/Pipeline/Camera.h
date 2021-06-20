#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vector.h"
namespace Sphynx::Core {
	class Camera
	{
	private:
		glm::vec3 Position;
		glm::vec4 Rotation;
		glm::mat4 ViewMatrix,ProjectionMatrix, RotationMatrix;
	public:
		Camera(glm::vec3 Position, glm::vec4 Rotation);
	};
}