#include "pch.h"
#include "VkWindow.h"
#include "GLFW/glfw3.h"
#include "VkRenderer.h"

using namespace Sphynx::Core;

void Sphynx::Core::Graphics::Vulkan::VkWindow::Internal_ChangeTitle(const char* title)
{
}

bool Sphynx::Core::Graphics::Vulkan::VkWindow::IsAlive()
{
	return false;
}

void Sphynx::Core::Graphics::Vulkan::VkWindow::OnClose()
{
}

void Sphynx::Core::Graphics::Vulkan::VkWindow::OnUpdate()
{
}

void Sphynx::Core::Graphics::Vulkan::VkWindow::OnResize(Events::OnWindowResize& e)
{
}

void* Sphynx::Core::Graphics::Vulkan::VkWindow::GetNativePointer()
{
	return nullptr;
}

bool Sphynx::Core::Graphics::Vulkan::VkWindow::IsVsyncEnabled()
{
	return false;
}

void Sphynx::Core::Graphics::Vulkan::VkWindow::SetVsync(bool vsync)
{
}

Sphynx::Core::Bounds Sphynx::Core::Graphics::Vulkan::VkWindow::GetBounds()
{
	return Bounds();
}

void Sphynx::Core::Graphics::Vulkan::VkWindow::SetClearColor(glm::vec4 color)
{
}

//Sphynx::Core::Graphics::IRenderer* Sphynx::Core::Graphics::Vulkan::VkWindow::GetRenderer()
//{
//	return Renderer;
//}

Sphynx::Core::Graphics::Vulkan::VkWindow::VkWindow(Application* App, Bounds WinBounds, std::string title, bool fullscreen)
{
	//This function will initialize volk,create a window and create a vulkan instance.
	

	if (WinBounds.Height == 0 || WinBounds.Width == 0) {
		WinBounds = DefBounds;
	}
	else if (title.empty()) {
		title = "Sphynx Engine";
	}
	Init(App, title, fullscreen);

	Height = WinBounds.Height;
	Width = WinBounds.Width;

	auto rslt = volkInitialize();
	if (rslt != VK_SUCCESS) {
		Core_Error("Unable to Initialize volk");
	}
	VkInstanceCreateInfo InstanceInfo;
	InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceInfo.flags = 0;
	auto ai = VkApplicationInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO ,NULL,"Sphynx Engine",Sphynx_Version_Num,title.c_str(),Sphynx_Version_Num,VK_API_VERSION_1_3 };
	InstanceInfo.pApplicationInfo = &ai;
	InstanceInfo.enabledLayerCount = InstanceInfo.enabledExtensionCount = 0;



	VkInstance Instance;
	rslt = vkCreateInstance(&InstanceInfo, nullptr, &Instance);
	if (rslt != VK_SUCCESS) {
		Core_Error("Unable to create a Vulkan Instance. Check Driver or Compatibilty");
	}
	volkLoadInstanceOnly(Instance);
	Renderer = new VkRenderer(&Instance);
	volkLoadDevice(Renderer->GetLogicalDevice(Renderer->GetPrimaryPhysicalDevice()));
	Window = glfwCreateWindow(Width, Height, Title.c_str(), NULL, NULL);
	//Set up viewport.
	Viewport vp;
	vp.Height = Height;
	vp.Width = Width;
	Renderer->SetViewPort(vp);
	glfwCreateWindowSurface(Instance, Window, NULL, &Surface);

}

Sphynx::Core::Graphics::Vulkan::VkWindow::~VkWindow()
{
}
