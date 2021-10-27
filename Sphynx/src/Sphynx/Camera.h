#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameObject.h"

namespace Sphynx {
	class Application;
}
namespace Sphynx {
	class Camera : public Component
	{
	private:
		static GameObject* PrimaryCamera;
		glm::mat4 ProjectionMatrix;
		bool IsOrtho;
		float Zoom;
		virtual void OnComponentAttach(GameObject* Parent);
		virtual void OnComponentDetach();
		static Sphynx::GameObject CreateCameraObject() {
			auto Obj = Sphynx::GameObject();
			Obj.AddComponent<Sphynx::Camera>();
			return Obj;
		}
	public:
		//Creates a Camera with Default Settings
		Camera();
		//FOV must be in degrees. This Constructor Creates a Perspective Camera
		Camera(float FOV, float NearClip, float FarClip);
		//Creates a Orthographic Camera
		Camera(float NearClip, float FarClip, float Width, float Height);
		virtual void Update();
		glm::mat4& GetProjectionMatrix() { return ProjectionMatrix; };
		glm::mat4& GetViewMatrix() { return GetGameObject()->GetTransform()->GetModelMatrix(); };
		bool IsOrthographic() { return IsOrtho; };
		//void SetOrthographic();
		//void SetPerspective();
		void SetViewMatrix(glm::mat4 mat) { GetGameObject()->GetTransform()->SetModelMatrix(mat); };
		static GameObject* GetPrimaryCamera();
		float GetZoom() { return Zoom; };
		float SetZoom(float zoom) { Zoom = zoom; };

		friend Application;
	};
}