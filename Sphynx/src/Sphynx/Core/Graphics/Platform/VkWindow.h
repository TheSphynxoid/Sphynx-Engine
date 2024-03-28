#pragma once
#include "../Window.h"
#include "VkRenderer.h"

namespace Sphynx::Core::Graphics::Vulkan {
	class VkRenderer;
	class VkWindow : public IWindow{
		// Inherited via IWindow
		virtual void Internal_ChangeTitle(const char* title) override;
		virtual bool IsAlive() override;
		virtual void OnClose() override;
		virtual void OnUpdate() override;
		virtual void OnResize(Events::OnWindowResize& e) override;
		//Return VkSurface.
		virtual void* GetNativePointer() override;
		virtual bool IsVsyncEnabled() override;
		virtual void SetVsync(bool vsync) override;
		virtual Bounds GetBounds() override;
		inline virtual int GetHeight() override { return Height; };
		inline virtual int GetWidth() override { return Width; };
		virtual void SetClearColor(glm::vec4 color) override;
		inline virtual Sphynx::Core::Graphics::IRenderer* GetRenderer() override { return Renderer; };
	public:
		VkWindow(Application* App, Bounds WinBounds, std::string title, bool fullscreen);
		~VkWindow();
	private:
		int Height, Width;
		VkRenderer* Renderer;
		VkSurfaceKHR Surface;
		GLFWwindow* Window;
	};
}
