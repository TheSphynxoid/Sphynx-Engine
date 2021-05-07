#include "pch.h"
#include "Imgui.h"
#pragma region ImGui Includes
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
#pragma endregion
#include "Application.h"
#include "Core/Graphics/Window.h"
#include "Events/Event.h"
#include "Logger.h"
#include <charconv>

using namespace Sphynx::Events;

//Because of the inablity to put this in DebugWindow,it's are here.
ImGuiTextBuffer     Buf;							 
//Because of the inablity to put this in DebugWindow,it's are here.
ImGuiTextFilter     Filter;							 
//Because of the inablity to put this in DebugWindow,it's are here.
ImVector<int>       LineOffsets;
//For Consistancy's sake this going to be ImVector and here.
ImVector<ImVec4>	Colors;


void Sphynx::Core::Imgui::Start(Application* app)
{
	App = app;
	window = app->GetMainWindow();
	time = app->GetTimeObject();
	window->GetEventSystem()->Subscribe<Imgui, Events::OnOverlayUpdate>(this, &Imgui::OnOverlayUpdate);
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
			//why does it happen? i don't know. i can't be bothered for now.It loses us one update cycle
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

void Sphynx::Core::Imgui::AddOverlayWindow(IOverlayWindow* Window)
{
	Overlays.push_back(Window);
	Names.push_back(typeid(*Window).name());
	Window->InternalSetup(this);
}

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
	window->GetEventSystem()->QueueEvent<Events::OnOverlayModuleDown>(Events::OnOverlayModuleDown());
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

ImVec4 GetColorLevel(spdlog::level::level_enum& le)
{
	switch (le)
	{
	default:
		return ImVec4(0, 0, 0, 1);
	case spdlog::level::info:
		return ImVec4(0, 0, 1, 1);
	case spdlog::level::trace:
		return ImVec4(0.5f, 0.5f, 0, 1);
	case spdlog::level::err:
		return ImVec4(1, 0, 0, 1);
	case spdlog::level::warn:
		return ImVec4(0, 0.5f, 0.5f, 1);
	case spdlog::level::debug:
		return ImVec4(1, 1, 1, 1);
	}
}

void Sphynx::Core::DebugWindow::OnEventLog(OnLog& e)
{
	int old_size = Buf.size();
	Buf.appendfv(e.msg.c_str(), NULL);
	for (int new_size = Buf.size(); old_size < new_size; old_size++) {
		if (Buf[old_size] == '\n') {
			LineOffsets.push_back(old_size + 1);
			Colors.push_back(GetColorLevel(e.level));
		}
	}
}

Sphynx::Core::DebugWindow::DebugWindow(Application* app)
{
	App = app;
	eventsystem = App->GetAppEventSystem();
	window = App->GetMainWindow();
	memset(TitleBuffer, 0, sizeof(TitleBuffer));
	LineOffsets.push_back(0);
	GlobalEventSystem::GetInstance()->Subscribe<DebugWindow, OnLog>(this, &DebugWindow::OnEventLog);
}

void HandleLogging(const char* text, int lvl, bool isClient)
{
	if (isClient) {
		switch (lvl)
		{
		default:
			break;
		case 0:
			Client_Trace(text);
			break;
		case 1:
			Client_Info(text);
			break;
		case 2:
			Client_Warn(text);
			break;
		case 3:
			Client_Error(text);
			break;
		case 4:
			Client_Trace("There no Debug For Client Log=", text);
			break;
		}
	}
	else {
		switch (lvl)
		{
		default:
			break;
		case 0:
			Core_Trace(text);
			break;
		case 1:
			Core_Info(text);
			break;
		case 2:
			Core_Warn(text);
			break;
		case 3:
			Core_Error(text);
			break;
		case 4:
			Core_Debug_Message(text);
			break;
		}
	}
}

void Sphynx::Core::DebugWindow::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(420, 360), ImGuiCond_Once);
	if (ImGui::Begin("Debug Window", &IsOpen, ImGuiWindowFlags_NoDocking)) {
		if (ImGui::CollapsingHeader("Logger")) {
			ImGui::Indent();
			static int item;
			static int rb;
			static char* lvls[5]{ "Trace","Info","Warn","Error","Debug" };
			if (ImGui::RadioButton("Core", rb == 0)) { rb = 0; } ImGui::SameLine();
			if (ImGui::RadioButton("Client", rb == 1)) { rb = 1; }
			if (ImGui::BeginCombo("Log Level", lvls[item])) {
				if (ImGui::Selectable("Trace", item == 0)) {
					item = 0;
				}
				if (ImGui::Selectable("Info", item == 1)) {
					item = 1;
				}
				if (ImGui::Selectable("Warn", item == 2)) {
					item = 2;
				}
				if (ImGui::Selectable("Error", item == 3)) {
					item = 3;
				}
				if (ImGui::Selectable("Debug", item == 4)) {
					item = 4;
				}
				ImGui::EndCombo();
			}
			static char buffer[512];
			if (ImGui::InputText("Log", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
				HandleLogging(buffer, item, rb);
				memset(buffer, 0, sizeof(buffer));
			}
			ImGui::Separator();
			//Got this from the demo.
			ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			const char* buf = Buf.begin();
			const char* buf_end = Buf.end();
			ImGuiListClipper clipper;
			clipper.Begin(LineOffsets.Size - 1);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, Colors[line_no]);
					ImGui::TextUnformatted(line_start, line_end);
					ImGui::PopStyleColor();
				}
			}
			clipper.End();
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader("ImGui")) {
			ImGui::Indent();
			if (ImGui::Button("Show Demo Window"))
			{
				GetImGui()->AddOverlayWindow(new Sphynx::Core::DemoWindow());
			}
			ImGui::Text("ImGui Windows:");
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