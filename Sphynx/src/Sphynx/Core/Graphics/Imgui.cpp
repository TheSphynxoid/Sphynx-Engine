#include "pch.h"
#include "Imgui.h"
#pragma region ImGui Includes
#include <imgui.h>
#ifdef IMGUI_GLFW && !defined(IMGUI_DX11)
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
#pragma endregion
#include "Application.h"
#include "Core/Graphics/Window.h"
#include "Events/Event.h"
#include "Logger.h"
#include <charconv>
#include "Platform/GLWindow.h"
#include "Core/SceneManager.h"
#include "Scene.h"
#include "Core/Graphics/Pipeline/FrameBuffer.h"
#include "Core/Graphics/Pipeline/Texture.h"

using namespace Sphynx::Events;

void Sphynx::Core::Imgui::ImGuiOnWindowShutdown(Sphynx::Events::OnWindowClose& e) {
	Shutdown();
}

void Sphynx::Core::Imgui::Start()
{
	App = GetApplication();
	window = GetMainWindow();
	window->GetEventSystem()->Subscribe<Events::OnWindowClose>(&Imgui::ImGuiOnWindowShutdown);
#ifdef IMGUI_DX11
	Imgui_ImplDX11_Init();
#else
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 0.56f, 0.43f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.12f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.04f, 0.25f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.19f, 0.48f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.04f, 0.31f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.03f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11f, 0.07f, 0.27f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.70f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.69f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 1.00f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.58f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	//ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetNativePointer(), true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
#endif
}

void Sphynx::Core::Imgui::OnOverlayUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	for (auto& w : Overlays) {
		if (w->IsOpen) {
			w->Draw();
		}
	}
	ImGui::EndFrame();
	ImGui::UpdatePlatformWindows();
	ImGui::Render();
	ImGui::GetDrawData();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::list<const char*> Sphynx::Core::Imgui::GetTypeInfoList()
{
	return Names;
}

void Sphynx::Core::Imgui::AddOverlayWindow(IOverlayWindow* Window)
{
	Overlays.push_back(Window);
	Names.push_back(typeid(*Window).name());
}

void Sphynx::Core::Imgui::RemoveOverlayWindow(IOverlayWindow* Window)
{
	Overlays.remove(Window);
	Window->Delete();
}

int Sphynx::Core::Imgui::GetNumberOfWindows()
{
	return Overlays.size();
}

void Sphynx::Core::Imgui::Shutdown()
{
	//window->GetEventSystem()->QueueEvent<Events::OnOverlayModuleDown>(Events::OnOverlayModuleDown());
	for (auto& w : Overlays) {
		if (w->IsOpen == false) {
			RemoveOverlayWindow(w);
			break;
		}
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();	
}

void Sphynx::Core::DemoWindow::Draw()
{
	ImGui::ShowDemoWindow(&IsOpen);
}

void Sphynx::Core::AboutWindow::Draw()
{
	ImGui::ShowAboutWindow(&IsOpen);
}

Sphynx::Core::DebugWindow::DebugWindow()
{
	App = GetApplication();
	eventsystem = App->GetAppEventSystem();
	window = App->GetMainWindow();
	memset(TitleBuffer, 0, sizeof(TitleBuffer));
}

//Long Shit for centralized debugging.
void Sphynx::Core::DebugWindow::Draw()
{
	if (ImGui::Begin("Debug Window", &IsOpen)) {
		if (ImGui::CollapsingHeader("ImGui")) {
			ImGui::Indent();
			if (ImGui::Button("Show Demo Window"))
			{
				Imgui::AddOverlayWindow(new Sphynx::Core::DemoWindow());
			}
			ImGui::Text("ImGui Windows:");
			auto ol = Imgui::GetTypeInfoList();
			if (ImGui::BeginListBox("##imguiwindows", ImVec2(FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 5)))
			{
				static int selected = -1;
				int i = 0;
				for (auto& name : ol) {
					//Text Formating
					std::array<char, 5> index = std::array<char,5>();
					auto [ptr, ec] = std::to_chars(index.data(), index.data() + 5, i);
					char d[256];
					memset(d, '\n', sizeof(char) * (strlen(name) + index.size()));
					strcpy_s(d, name);
					strcat_s(d, " ##");
					strcat_s(d, index.data());
					//End Text Formating
					if (ImGui::Selectable(d, selected == i)) {
						selected = i;
					}
					i++;
				}
				ImGui::EndListBox();
			}
			ImGui::Text("Number Of Window = %i", Imgui::GetNumberOfWindows());
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader("Window")) {
			ImGui::Indent();
			ImGui::Text("Main Window Width:%i", window->GetWidth());
			ImGui::Text("Main Window Height:%i", window->GetHeight());
			auto fb = Sphynx::Core::SceneManager::GetScene().GetPrimaryCamera()->GetFrameBuffer();
			ImGui::Text("Camera FrameBuffer Width:%i", fb->GetColorAttachment(0)->GetWidth());
			ImGui::Text("Camera FrameBuffer Height:%i", fb->GetColorAttachment(0)->GetHeight());
			bool vs = window->IsVsyncEnabled();
			if (ImGui::Checkbox("Vsync", &vs)) {
				window->SetVsync(vs);
			}
			ImGui::InputText("Window Title", TitleBuffer, sizeof(TitleBuffer));
			if (ImGui::Button("Set Title")) {
				window->ChangeTitle(TitleBuffer);
			}
			static int bnds[2];
			ImGui::InputInt2("Window Size", bnds);
			if (ImGui::Button("Set Size")) {
				window->Resize(bnds[0], bnds[1]);
			}
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader("Time")) {
			ImGui::Indent();
			ImGui::Text("Delta Time:%f", Time::GetDeltaTime());
			ImGui::Text("Ticks:%llu", Time::GetTicks());
			ImGui::Text("FPS:%i", (int)(1 / Time::GetDeltaTime()));
			ImGui::Unindent();
		}
		ImGui::End();
	}
}