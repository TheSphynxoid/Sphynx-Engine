#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameObject.h"
#include "Events/WindowEvents.h"
#include "Core/Graphics/Pipeline/Viewport.h"

namespace Sphynx {
	class Application;
	class MeshRenderer;
	namespace Core::Graphics {
		class FrameBuffer;
		class Texture;
	}
}
namespace Sphynx {
	class Camera : public Component
	{
	private:
		glm::mat4 ProjectionMatrix;
		bool IsOrtho;
		float NearClip, FarClip;
		float FOV;
		float Zoom;
		Core::Graphics::Viewport CamViewport = Core::Graphics::Viewport();
		Core::Graphics::FrameBuffer* RenderTarget = nullptr;
		Core::Graphics::Texture *ColorTex, *DepthTex = nullptr;
		void ResizeWindow(Events::OnWindowResize& e);
		virtual void OnComponentAttach(GameObject* Parent);
		virtual void OnComponentDetach();
	public:
		//Creates a Camera with Default Settings
		Camera();
		//FOV must be in degrees. This Constructor Creates a Perspective Camera
		Camera(float fov, float aspectRatio, float nearClip, float farClip);
		//Creates a Orthographic Camera
		//Camera(float nearClip, float farClip, float width, float height);
		virtual void Update();
		glm::mat4& GetProjectionMatrix() { return ProjectionMatrix; };
		glm::mat4& GetViewMatrix() { return GetGameObject()->GetTransform()->GetModelMatrix(); };
		bool IsOrthographic() { return IsOrtho; };
		//void SetOrthographic();
		//void SetPerspective();
		void SetViewMatrix(glm::mat4 mat) { GetGameObject()->GetTransform()->SetModelMatrix(mat); };
		void SetFrameBuffer(Sphynx::Core::Graphics::FrameBuffer* fb);
		void SetViewport(Sphynx::Core::Graphics::Viewport v);
		float GetZoom() { return Zoom; };
		float SetZoom(float zoom) { Zoom = zoom; };
		//Will Return Null if the Camera has no RenderTarget(FrameBuffer).
		Core::Graphics::FrameBuffer* GetFrameBuffer() { return RenderTarget; };
		Core::Graphics::Viewport& GetViewport() {
			return CamViewport;
		}
		friend Application;
	};
}