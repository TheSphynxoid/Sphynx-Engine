#ifndef Sphynx_Transform
#define Sphynx_Transform
#include "Component.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/glm.hpp"
#include "Events/Event.h"

namespace Sphynx {
	class Transform : public Component {
	private:
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
		glm::vec3 Position = glm::vec3(0.0f);
		virtual void OnComponentAttach(GameObject* parent) override;
		virtual void OnComponentDetach() override;
	public:
		Transform();
		Transform(glm::vec3 pos, glm::vec3 scale, glm::quat rot);
		glm::mat4& GetModelMatrix() { return ModelMatrix; };
		void SetModelMatrix(glm::mat4 mat) { ModelMatrix = mat; };
		void Translate(glm::vec3 towards);
		void Rotate(float Angle, glm::vec3 axis);
		void Scale(glm::vec3 scale);
		void SetPosition(glm::vec3 to);
		void SetRotation(glm::vec3 rot);
		glm::vec3& GetPosition();
		virtual ~Transform();
		virtual void Update() override;
	};
}
#else
#endif