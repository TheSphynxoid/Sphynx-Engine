#include "pch.h"
#include "Imgui.h"
#include <imgui.h>
#ifdef IMGUI_GLFW
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#elif defined(IMGUI_DX11) && !defined(IMGUI_GLFW)
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#else
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "GLFW/glfw3.h"
#endif
#include "Application.h"
#include "Window/Window.h"

bool b = true;

void Sphynx::Core::Imgui::Start(Application* app)
{
	App = app;
	window = app->GetAppWindow();
	time = app->GetTimeObject();
	app->GetAppEventSystem()->Subscribe<Imgui, Events::OnOverlayUpdate>(this, &Imgui::OnOverlayUpdate);
#ifdef IMGUI_DX11
	Imgui_ImplDX11_Init();
#else
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.DisplaySize = ImVec2(window->GetWidth(), window->GetHeight());
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetNativePointer(), true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
#endif
}

void Sphynx::Core::Imgui::OnOverlayUpdate(Events::OnOverlayUpdate& e)
{
	//ImGuiIO& io = ImGui::GetIO();
//io.DeltaTime = time.GetDeltaTime();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	for (auto& w : Overlays) {
		w->Draw();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Sphynx::Core::Imgui::Update()
{
}

void Sphynx::Core::Imgui::AddOverlayWindow(IOverlayWindow* Window)
{
	Overlays.push_back(Window);
}

void Sphynx::Core::Imgui::RemoveOverlayWindow(IOverlayWindow* Window)
{
	Overlays.remove(Window);
}

void Sphynx::Core::Imgui::Shutdown()
{
	App->GetAppEventSystem()->Dispatch<Events::OnOverlayModuleDown>(Events::OnOverlayModuleDown());
}

void Sphynx::Core::DemoWindow::Draw()
{
	ImGui::ShowDemoWindow();
}

void Sphynx::Core::AboutWindow::Draw()
{
	ImGui::ShowAboutWindow();
}
