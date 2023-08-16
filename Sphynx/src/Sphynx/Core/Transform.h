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
		glm::quat Rotation = glm::quat(1, 0, 0, 0);
		glm::vec3 Scaling = glm::vec3(1.0f);
		glm::vec3 Origin = glm::vec3(0.0f);
		virtual void OnComponentAttach(GameObject* parent) override;
		virtual void OnComponentDetach() override;
		void CalcModelMatrix();
	public:
		CompImpl(Transform);
		//You Can Force Matrix Calculations.
		bool Changed = true;
		Transform();
		Transform(glm::vec3 pos, glm::vec3 scale, glm::quat rot);
		glm::mat4& GetModelMatrix() { return ModelMatrix; };
		void SetModelMatrix(glm::mat4 mat) { ModelMatrix = mat; };
		void Translate(glm::vec3 towards);
		void Rotate(float Angle, glm::vec3 axis);
		void Scale(glm::vec3 scale);
		void SetPosition(glm::vec3 to);
		void SetRotation(glm::vec3 rot);
		void SetScale(glm::vec3 scale);
		void SetOrigin(glm::vec3 origin);
		glm::vec3 GetPosition();
		glm::vec3 GetScale();
		glm::quat GetRotation();
		virtual ~Transform();
		virtual void Update() override;
	};
}
#else
#endif