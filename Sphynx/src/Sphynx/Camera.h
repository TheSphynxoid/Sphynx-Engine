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
		glm::mat4 ViewMatrix;
		bool IsOrtho;
		float NearClip, FarClip, AspectRatio;
		float FOV;
		float Zoom = 0;
		Core::Graphics::Viewport CamViewport = Core::Graphics::Viewport();
		Core::Graphics::FrameBuffer* RenderTarget = nullptr;
		Core::Graphics::Texture *ColorTex, *DepthTex = nullptr;
		void ResizeWindow(Events::OnWindowResize& e);
		virtual void OnComponentAttach(GameObject* Parent);
		virtual void OnComponentDetach();
	public:
		CompImpl(Camera);
		//Untested ,Probably Works.
		virtual Component* CopyInstance() override {
			return new Camera((Camera&)*this);
		}
		//Creates a Camera with Default Settings
		Camera();
		//FOV must be in degrees. This Constructor Creates a Perspective Camera
		Camera(float fov, float aspectRatio, float nearClip, float farClip);
		//Creates a Orthographic Camera
		//Camera(float nearClip, float farClip, float width, float height);
		virtual void Update();
		glm::mat4& GetProjectionMatrix() { return ProjectionMatrix; };
		glm::mat4& GetViewMatrix() { return ViewMatrix; };
		bool IsOrthographic() { return IsOrtho; };
		void SetToOrthographic(float Width, float Height, float nearclip, float farclip);
		void SetToPerspective(float fov, float aspectRatio, float nearClip, float farClip);
		void SetViewMatrix(glm::mat4 mat) {
			GetGameObject()->GetTransform()->SetModelMatrix(mat); 
		};
		void SetAspectRatio(float aspectRatio);
		void SetFrameBuffer(Sphynx::Core::Graphics::FrameBuffer* fb);
		void SetViewport(Sphynx::Core::Graphics::Viewport v);
		float GetZoom() { return Zoom; };
		float SetZoom(float zoom) { Zoom = zoom; };
		float GetFOV() { return FOV; };
		float GetNearClip() { return NearClip; };
		float GetFarClip() { return FarClip; };
		float GetAspectRatio() { return AspectRatio; };
		//Will Return Null if the Camera has no RenderTarget(FrameBuffer).
		Core::Graphics::FrameBuffer* GetFrameBuffer() { return RenderTarget; };
		Core::Graphics::Viewport& GetViewport() {
			return CamViewport;
		}
		friend Application;
	};

	COMPONENT_REGISTER(Camera);

	struct OnFrameResize : public Events::Event {
		glm::vec2 OldDimensions;
		glm::vec2 NewDimensions;
		Camera* Cam;
		Core::Graphics::FrameBuffer* Frame;

		OnFrameResize(glm::vec2 old, glm::vec2 curr, Camera* cam, Core::Graphics::FrameBuffer* frame) 
			: OldDimensions(old), NewDimensions(curr), Cam(cam), Frame(frame) {};
	};
	struct OnFrameChange : public Events::Event {

		Camera* Cam;
		Core::Graphics::FrameBuffer* Frame;

		OnFrameChange(Camera* cam, Core::Graphics::FrameBuffer* frame) : Cam(cam), Frame(frame) {};
	};
}