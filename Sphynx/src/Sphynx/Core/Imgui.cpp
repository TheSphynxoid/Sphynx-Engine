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
#include <charconv>

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
		if (w->IsOpen == false) {
			RemoveOverlayWindow(w);
			//Hack or solution... Idk. it stops the iterator from causing us to use the deleted window. 
			//why does it happen? i don't know. i can't be bothered for now.
			break;
		}
	}
	ImGui::EndFrame();
	ImGui::UpdatePlatformWindows();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Sphynx::Core::Imgui::Update()
{

}

std::list<const char*> Sphynx::Core::Imgui::GetTypeInfoList()
{
	return Names;
}

//void Sphynx::Core::Imgui::AddOverlayWindow(IOverlayWindow* Window)
//{
//	Overlays.push_back(Window);
//	Names.push_back(typeid(Window).name());
//	Window->InternalSetup(this);
//}

void Sphynx::Core::Imgui::RemoveOverlayWindow(IOverlayWindow* Window)
{
	Overlays.remove(Window);
	Window->Close();
}

int Sphynx::Core::Imgui::GetNumberOfWindows()
{
	return Overlays.size();
}

void Sphynx::Core::Imgui::Shutdown()
{
	App->GetAppEventSystem()->Dispatch<Events::OnOverlayModuleDown>(Events::OnOverlayModuleDown());
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

void Sphynx::Core::IOverlayWindow::InternalSetup(Imgui* instance)
{
	imgui = instance;
}

Sphynx::Core::DebugWindow::DebugWindow(Application* app)
{
	App = app;
	eventsystem = App->GetAppEventSystem();
	window = App->GetAppWindow();
	memset(TitleBuffer, 0, sizeof(TitleBuffer));
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_Always);
}


void Sphynx::Core::DebugWindow::Draw() {

	if (ImGui::Begin("Application Center", &IsOpen, ImGuiWindowFlags_NoDocking)) {
		if (ImGui::CollapsingHeader("ImGui")) {
			ImGui::Indent();
			if (ImGui::Button("Show Demo Window"))
			{
				GetImGui()->AddOverlayWindow(new Sphynx::Core::DemoWindow());
			}
			ImGui::Text("ImGui Windows");
			auto* imgui = GetImGui();
			auto ol = imgui->GetTypeInfoList();
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
					strcpy(d, name);
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
			ImGui::Text("Number Of Window = %i", GetImGui()->GetNumberOfWindows());
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader("Window")) {
			ImGui::Indent();
			ImGui::Text("Window Height:%i", window->GetHeight());
			ImGui::Text("Window Width:%i", window->GetWidth());
			bool vs = window->IsVsyncEnabled();
			if (ImGui::Checkbox("Vsync", &vs)) {
				window->SetVsync(vs);
			}
			ImGui::InputText("Window Title", TitleBuffer, sizeof(TitleBuffer));
			if (ImGui::Button("Set Title")) {
				window->ChangeTitle(TitleBuffer);
			}
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader("Time")) {
			ImGui::Indent();
			ImGui::Text("Activity Time:%f", App->GetTimeObject()->GetActivityTime());
			ImGui::Text("Delta Time:%d", App->GetTimeObject()->GetDeltaTime());
			ImGui::Unindent();
		}
		ImGui::End();
	}
}